from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7

#### TIME MONITORING START ####

# Time control starts
import time as timer
print(timer.ctime())
# Measure process time
t0p = timer.clock()
# Measure wall time
t0w = timer.time()

def StartTimeMeasuring():
    # Measure process time
    time_ip = timer.clock()
    return time_ip

def StopTimeMeasuring(time_ip, process, report):
    # Measure process time
    time_fp = timer.clock()
    if( report ):
        used_time = time_fp - time_ip
        print("::[KSM Simulation]:: [ %.2f" % round(used_time,2),"s", process," ] ")

#### TIME MONITORING END ####

# Import kratos core and applications
from KratosMultiphysics import *
from KratosMultiphysics.StructuralMechanicsApplication import *
from KratosMultiphysics.ExternalSolversApplication import *
from KratosMultiphysics.ShapeOptimizationApplication import *

import math

######################################################################################
######################################################################################
######################################################################################

#### PARSING THE PARAMETERS ####

# Import define_output
parameter_file = open("ProjectParameters.json",'r')
#parameter_file = open("ProjectParameters_dynamic.json",'r')
ProjectParameters = Parameters( parameter_file.read())

parallel_type = ProjectParameters["problem_data"]["parallel_type"].GetString()
if parallel_type == "MPI":
    import KratosMultiphysics.mpi as KratosMPI

# Set echo level
verbosity = ProjectParameters["problem_data"]["echo_level"].GetInt()

#### Model_part settings start ####

# Defining the model_part
main_model_part = ModelPart(ProjectParameters["problem_data"]["model_part_name"].GetString())
main_model_part.ProcessInfo.SetValue(DOMAIN_SIZE, ProjectParameters["problem_data"]["domain_size"].GetInt())

# Construct the solver (main setting methods are located in the solver_module)
solver_module = __import__(ProjectParameters["solver_settings"]["solver_type"].GetString())
solver = solver_module.CreateSolver(main_model_part, ProjectParameters["solver_settings"])

# Add variables (always before importing the model part) (it must be integrated in the ImportModelPart)
# If we integrate it in the model part we cannot use combined solvers
print("before adding variables")
solver.AddVariables()
print("after adding variables")

# Read model_part (note: the buffer_size is set here) (restart can be read here)
solver.ImportModelPart()

# Add dofs (always after importing the model part) (it must be integrated in the ImportModelPart)
# If we integrate it in the model part we cannot use combined solvers
solver.AddDofs()

# Creation of Kratos model
SolidModel = Model()
SolidModel.AddModelPart(main_model_part)

# Build sub_model_parts or submeshes (rearrange parts for the application of custom processes)
## Get the list of the submodel part in the object Model
for i in range(ProjectParameters["solver_settings"]["processes_sub_model_part_list"].size()):
    part_name = ProjectParameters["solver_settings"]["processes_sub_model_part_list"][i].GetString()
    SolidModel.AddModelPart(main_model_part.GetSubModelPart(part_name))

#print model_part and properties
if(verbosity>1):
    print("")
    print(main_model_part)
    for properties in main_model_part.Properties:
        print(properties)

#### Model_part settings end ####

#### Processes settings start ####

#obtain the list of the processes to be applied

import process_factory
list_of_processes = process_factory.KratosProcessFactory(SolidModel).ConstructListOfProcesses( ProjectParameters["constraints_process_list"] )

list_of_processes += process_factory.KratosProcessFactory(SolidModel).ConstructListOfProcesses( ProjectParameters["loads_process_list"] )

#list_of_processes += process_factory.KratosProcessFactory(SolidModel).ConstructListOfProcesses(ProjectParameters["list_other_processes"])

# Print list of constructed processes
if(verbosity>1):
    for process in list_of_processes:
        print(process)

for process in list_of_processes:
    process.ExecuteInitialize()

#### processes settings end ####

#### START SOLUTION ####

computing_model_part = solver.GetComputingModelPart()

#### Output settings start ####

problem_path = os.getcwd()
problem_name = ProjectParameters["problem_data"]["problem_name"].GetString()
output_settings = ProjectParameters["output_configuration"]

# Initialize GiD  I/O (gid outputs, file_lists)
if parallel_type == "OpenMP":
    from gid_output_process import GiDOutputProcess
    gid_output = GiDOutputProcess(computing_model_part,
                                  problem_name,
                                  output_settings)
elif parallel_type == "MPI":
    from gid_output_process_mpi import GiDOutputProcessMPI
    gid_output = GiDOutputProcessMPI(computing_model_part,
                                     problem_name,
                                     output_settings)

gid_output.ExecuteInitialize()

#### Output settings end ####

## Sets strategies, builders, linear solvers, schemes and solving info, and fills the buffer
solver.Initialize()

if (parallel_type == "OpenMP") or (KratosMPI.mpi.rank == 0):
    print(" ")
    print("::[KSM Simulation]:: Analysis -START- ")

for process in list_of_processes:
    process.ExecuteBeforeSolutionLoop()

## Set results when are written in a single file
gid_output.ExecuteBeforeSolutionLoop()

## Stepping and time settings (get from process info or solving info)
#delta time
delta_time = ProjectParameters["problem_data"]["time_step"].GetDouble()
#start step
step       = 0
#start time
time       = ProjectParameters["problem_data"]["start_time"].GetDouble()
#end time
end_time   = ProjectParameters["problem_data"]["end_time"].GetDouble()

# monitoring info:  # must be contained in the solver
#import solving_info_utility as solving_info_utils
#solving_info = solving_info_utils.SolvingInfoUtility(model_part)

# writing a initial state results file (if no restart)
# gid_io.write_results(time, computing_model_part) done in ExecuteBeforeSolutionLoop()

## Writing the full ProjectParameters file before solving
if ((parallel_type == "OpenMP") or (KratosMPI.mpi.rank == 0)) and (verbosity > 0):
    f = open("ProjectParametersOutput.json", 'w')
    f.write(ProjectParameters.PrettyPrintJsonString())
    f.close()

# Solving the problem (time integration)
while(time <= end_time):

    #TODO: this must be done by a solving_info utility in the solver
    # Store previous time step
    #~ computing_model_part.ProcessInfo[PREVIOUS_DELTA_TIME] = delta_time
    # Set new time step ( it can change when solve is called )
    #~ delta_time = computing_model_part.ProcessInfo[DELTA_TIME]

    time = time + delta_time
    step = step + 1
    main_model_part.ProcessInfo[TIME_STEPS] = step
    main_model_part.CloneTimeStep(time)

    if (parallel_type == "OpenMP") or (KratosMPI.mpi.rank == 0):
        print(" [STEP:",step," TIME:",time,"]")

    # Print process info

    for process in list_of_processes:
        process.ExecuteInitializeSolutionStep()

    gid_output.ExecuteInitializeSolutionStep()
    print("START SOLVING")
    solver.Solve()
    print("END SOLVING")
   
    eig = False
    if eig:
        eigenvector = main_model_part.ProcessInfo[EIGENVALUE_VECTOR]
        print(eigenvector)
        NumEigenvalues = len(eigenvector)
        for step in range(NumEigenvalues):
            eigenfrequency = math.sqrt(eigenvector[step])/2/math.pi
            main_model_part.ProcessInfo[TIME] = float(eigenfrequency)
            EigenvectorToSolutionStepVariableTransferUtility().Transfer(main_model_part,step,0)
            gid_output.PrintOutput()

    gid_output.ExecuteFinalizeSolutionStep()

    for process in list_of_processes:
        process.ExecuteFinalizeSolutionStep()

    for process in list_of_processes:
        process.ExecuteBeforeOutputStep()

    # Write results and restart files: (frequency writing is controlled internally by the gid_io)
    if gid_output.IsOutputStep():
        gid_output.PrintOutput()

    for process in list_of_processes:
        process.ExecuteAfterOutputStep()


# initialize response function(s), compute function value and gradient and write gradient output
# numberOfObjectives = ProjectParameters["optimization_settings"]["objectives"].size()
# for objectiveNumber in range(numberOfObjectives):
    main_model_part.AddNodalSolutionStepVariable(ADJOINT_NODE_COORD)
    MyResponseFunction = ReworkStrainEnergyResponseFunction(main_model_part,ProjectParameters["optimization_settings"]["objectives"][0]) 
    MyResponseFunction.CalculateValue()
    my_resp_value = MyResponseFunction.GetValue()
    MyResponseFunction.UpdateSensitivities()
    print("my response function value = ", my_resp_value )

    main_model_part.AddNodalSolutionStepVariable(STRAIN_ENERGY_SHAPE_GRADIENT)
    MyResponseFunction2 = StrainEnergyResponseFunction(main_model_part,ProjectParameters["optimization_settings"]["objectives"][0]) 
    MyResponseFunction2.calculate_value()
    my_resp_value_reference = MyResponseFunction2.get_value()
    MyResponseFunction2.calculate_gradient()
    print("my response function refernce value = ", my_resp_value_reference )

    print("I am ready with the sensitivty analysis computations")
   

#eigen_values = [ev for ev in main_model_part.ProcessInfo[EIGENVALUE_VECTOR]]
#print ("The Eigen Values are:")
#print (eigen_values)
#eigen_utility = EigenvectorToSolutionStepVariableTransferUtility()
#for step in range(len(eigen_values)):
   # main_model_part.ProcessInfo[TIME] = float(step+1)
    #eigen_utility.Transfer(main_model_part,step,0)
    #gid_output.PrintOutput()



# Ending the problem (time integration finished)
#gid_output.ExecuteFinalize()

#for process in list_of_processes:
   # process.ExecuteFinalize()

# Check solving information for any problem
#~ solver.InfoCheck() # InfoCheck not implemented yet.

#### END SOLUTION ####

#if (parallel_type == "OpenMP") or (KratosMPI.mpi.rank == 0):
    #print("::[KSM Simulation]:: Analysis -END- ")
   # print(" ")

    # Measure process time
   # tfp = timer.clock()
    # Measure wall time
  #  tfw = timer.time()

  #  print("::[KSM Simulation]:: [ Computing Time = (%.2f" % (tfp - t0p)," seconds process time) ( %.2f" % (tfw - t0w)," seconds wall time) ]")

   # print(timer.ctime())
#print(main_model_part.ProcessInfo[EIGENVALUE_VECTOR])



from gid_output import GiDOutput
listOfNodalResults = ["STRAIN_ENERGY_SHAPE_GRADIENT"]
resultsDirectory = ""
designHistoryFilename = "result_gradient_reference"
designHistoryFilenameWithPath =  resultsDirectory+"/"+designHistoryFilename
gidIO = GiDOutput( "result_gradient_reference",
                     True,
                    "Binary",
                    "Single",
                     True,
                     True )
iteratorForInitialDesign = 0
gidIO.initialize_results( main_model_part )
gidIO.write_results( 1, main_model_part, listOfNodalResults, [] )     
      

#gidIO.finalize_results()  

from gid_output import GiDOutput
listOfNodalResults = ["ADJOINT_NODE_COORD"]
resultsDirectory = ""
designHistoryFilename = "result_gradient"
designHistoryFilenameWithPath =  resultsDirectory+"/"+designHistoryFilename
gidIO = GiDOutput( "result_gradient",
                     True,
                    "Binary",
                    "Single",
                     True,
                     True )
iteratorForInitialDesign = 0
gidIO.initialize_results( main_model_part )
gidIO.write_results( 1, main_model_part, listOfNodalResults, [] )     
      

gidIO.finalize_results()  

print("I finished the output process")

