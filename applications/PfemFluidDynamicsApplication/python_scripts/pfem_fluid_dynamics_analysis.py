from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7

#Activate it to import in the gdb path:
#import sys
#sys.path.append('/home/cpuigbo/kratos')
#x = input("stopped to allow debug: set breakpoints and press enter to continue");
import time as timer
# Import system python
import os

# Import kratos core and applications
import KratosMultiphysics
import KratosMultiphysics.ExternalSolversApplication
import KratosMultiphysics.DelaunayMeshingApplication
import KratosMultiphysics.PfemFluidDynamicsApplication
import KratosMultiphysics.SolidMechanicsApplication

from KratosMultiphysics.analysis_stage import AnalysisStage

class PfemFluidDynamicsAnalysis(AnalysisStage):

    def __init__(self, model, parameters):

        self.model = model

        #### TIME MONITORING START ####

        # Time control starts
        print(timer.ctime())
        # Measure process time
        self.t0p = timer.clock()
        # Measure wall time
        self.t0w = timer.time()
        #### TIME MONITORING END ####


        #### PARSING THE PARAMETERS ####

        #set echo level
        self.echo_level = parameters["problem_data"]["echo_level"].GetInt()

        # Print solving time
        self.report = False
        if( self.echo_level > 0 ):
            self.report = True

        print(" ")

        # defining the number of threads:
        num_threads = parameters["problem_data"]["threads"].GetInt()
        self.SetParallelSize(num_threads)
        print("::[KPFEM Simulation]:: [OMP USING",num_threads,"THREADS ]")
        #parallel.PrintOMPInfo()


        print(" ")
        print("::[KPFEM Simulation]:: [Time Step:", parameters["solver_settings"]["time_stepping"]["time_step"].GetDouble()," echo:", self.echo_level,"]")

        #### Model_part settings start ####
        super(PfemFluidDynamicsAnalysis,self).__init__(model,parameters)
        # Defining the model_part
        self.main_model_part = self.model.GetModelPart(parameters["solver_settings"]["model_part_name"].GetString())

        self.main_model_part.ProcessInfo.SetValue(KratosMultiphysics.SPACE_DIMENSION, parameters["solver_settings"]["domain_size"].GetInt())
        self.main_model_part.ProcessInfo.SetValue(KratosMultiphysics.DOMAIN_SIZE, parameters["solver_settings"]["domain_size"].GetInt())
        self.main_model_part.ProcessInfo.SetValue(KratosMultiphysics.DELTA_TIME, parameters["solver_settings"]["time_stepping"]["time_step"].GetDouble())
        self.main_model_part.ProcessInfo.SetValue(KratosMultiphysics.TIME, parameters["problem_data"]["start_time"].GetDouble())
        if( parameters["problem_data"].Has("gravity_vector") ):
             self.main_model_part.ProcessInfo.SetValue(KratosMultiphysics.GRAVITY_X, parameters["problem_data"]["gravity_vector"][0].GetDouble())
             self.main_model_part.ProcessInfo.SetValue(KratosMultiphysics.GRAVITY_Y, parameters["problem_data"]["gravity_vector"][1].GetDouble())
             self.main_model_part.ProcessInfo.SetValue(KratosMultiphysics.GRAVITY_Z, parameters["problem_data"]["gravity_vector"][2].GetDouble())

        self.problem_path = os.getcwd()
        self.problem_name = parameters["problem_data"]["problem_name"].GetString()

    def _CreateSolver(self):

        solver_module = __import__(self.project_parameters["solver_settings"]["solver_type"].GetString())
        return solver_module.CreateSolver(self.model, self.project_parameters["solver_settings"])

    def AddNodalVariablesToModelPart(self):

        # Add PfemSolidMechanicsApplication Variables
        import pfem_variables
        pfem_variables.AddVariables(self.main_model_part)


    def Run(self):

        self.Initialize()

        self.RunMainTemporalLoop()

        self.Finalize()


    def Initialize(self):

        # Add variables (always before importing the model part)
        self.AddNodalVariablesToModelPart()

        # Read model_part (note: the buffer_size is set here) (restart is read here)
        self._solver.ImportModelPart()

        # Add dofs (always after importing the model part)
        if((self.main_model_part.ProcessInfo).Has(KratosMultiphysics.IS_RESTARTED)):
            if(self.main_model_part.ProcessInfo[KratosMultiphysics.IS_RESTARTED] == False):
                self._solver.AddDofs()
        else:
            self._solver.AddDofs()

        #### Model_part settings end ####


        #print model_part and properties
        if(self.echo_level>1):
            print("")
            print(self.main_model_part)
            for properties in self.main_model_part.Properties:
                print(properties)

        #### Processes settings start ####

        #obtain the list of the processes to be applied
        import process_handler

        process_parameters = KratosMultiphysics.Parameters("{}")
        process_parameters.AddValue("echo_level", self.project_parameters["problem_data"]["echo_level"])
        # process_parameters.AddValue("constraints_process_list", self.project_parameters["constraints_process_list"])
        # process_parameters.AddValue("loads_process_list", self.project_parameters["loads_process_list"])
        if( self.project_parameters.Has("problem_process_list") ):
            process_parameters.AddValue("problem_process_list", self.project_parameters["problem_process_list"])
        # if( self.project_parameters.Has("output_process_list") ):
        #     process_parameters.AddValue("output_process_list", self.project_parameters["output_process_list"])
        # if( self.project_parameters.Has("processes_sub_model_part_tree_list") ):
        #     process_parameters.AddValue("processes_sub_model_part_tree_list",self.project_parameters["processes_sub_model_part_tree_list"])
        # if( self.project_parameters.Has("check_process_list") ):
        #     process_parameters.AddValue("check_process_list", self.project_parameters["check_process_list"])

        self.model_processes = process_handler.ProcessHandler(self.model, process_parameters)

        self.model_processes.ExecuteInitialize()

        ##here we initialize user-provided processes
        order_processes_initialization = self._GetOrderOfProcessesInitialization()
        self._list_of_processes        = self._CreateProcesses("processes", order_processes_initialization)

        for process in self._GetListOfProcesses():
            process.ExecuteInitialize()

        #### processes settings end ####


        # --PLOT GRAPHS OPTIONS START--###############
        #self.problem_path = os.getcwd() #current path
        #plot_active = general_variables.PlotGraphs
        #graph_plot = plot_utils.GraphPlotUtility(model_part, self.problem_path)
        # --PLOT GRAPHS OPTIONS END--#################

        #### START SOLUTION ####

        self.computing_model_part = self._solver.GetComputingModelPart()

        self.graphical_output = self.SetGraphicalOutput()

        ## Sets strategies, builders, linear solvers, schemes and solving info, and fills the buffer
        self._solver.Initialize()
        self._solver.InitializeStrategy()
        self._solver.SetEchoLevel(self.echo_level)



        # Initialize GiD  I/O (gid outputs, file_lists)
        self.GraphicalOutputExecuteInitialize()

        #### Output settings end ####

        # writing a initial state results file
        current_id = 0
        #if(load_restart == False):
        #    if (general_variables.TryToSetTheWeight):
        #        if (general_variables.TryToSetConstantWeight):
        #            conditions.SetConstantWeight( general_variables.TryToSetWeightVertical, general_variables.TryToSetWeightHorizontal);
        #        else:
        #            conditions.SetWeight();

        # set solver info starting parameters
        # solving_info = solving_info_utils.SolvingInfoUtility(model_part, SolverSettings)

        print(" ")
        print("::[KPFEM Simulation]:: Analysis -START- ")

        self.model_processes.ExecuteBeforeSolutionLoop()

        for process in self._GetListOfProcesses():
            process.ExecuteBeforeSolutionLoop()

        self.GraphicalOutputExecuteBeforeSolutionLoop()

        # Set time settings
        self.step       = self.main_model_part.ProcessInfo[KratosMultiphysics.STEP]
        self.time       = self.main_model_part.ProcessInfo[KratosMultiphysics.TIME]

        self.end_time   = self.project_parameters["problem_data"]["end_time"].GetDouble()
        self.delta_time = self.project_parameters["solver_settings"]["time_stepping"]["time_step"].GetDouble()


    def RunMainTemporalLoop(self):

        # Solving the problem (time integration)
        while(self.time < self.end_time):

            self.InitializeSolutionStep()
            self.SolveSolutionStep()
            self.FinalizeSolutionStep()


    def InitializeSolutionStep(self):

        self.clock_time = self.StartTimeMeasuring()

        # current time parameters
        # self.main_model_part.ProcessInfo.GetPreviousSolutionStepInfo()[KratosMultiphysics.DELTA_TIME] = self.delta_time
        self.delta_time = self.main_model_part.ProcessInfo[KratosMultiphysics.DELTA_TIME]

        self.time = self.time + self.delta_time
        self.step = self.step + 1

        self.main_model_part.ProcessInfo[KratosMultiphysics.STEP] = self.step
        self.main_model_part.CloneTimeStep(self.time)

        #print(" [STEP:",self.step," TIME:",self.time,"]")

        # processes to be executed at the begining of the solution step
        self.model_processes.ExecuteInitializeSolutionStep()

        for process in self._GetListOfProcesses():
            process.ExecuteInitializeSolutionStep()

        self.GraphicalOutputExecuteInitializeSolutionStep()

        # solve time step
        self._solver.InitializeSolutionStep()

        self.StopTimeMeasuring(self.clock_time,"Initialize Step" , self.report)

    def SolveSolutionStep(self):

        self.clock_time = self.StartTimeMeasuring()

        self._solver.Predict()

        self._solver.SolveSolutionStep()

        self._solver.FinalizeSolutionStep()

        self.StopTimeMeasuring(self.clock_time,"Solve Step" , self.report)

    def FinalizeSolutionStep(self):

        self.clock_time = self.StartTimeMeasuring()

        self.GraphicalOutputExecuteFinalizeSolutionStep()

        # processes to be executed at the end of the solution step
        self.model_processes.ExecuteFinalizeSolutionStep()

        for process in self._GetListOfProcesses():
            process.ExecuteFinalizeSolutionStep()

        #if (self.time>0.0001):
        # processes to be executed before writing the output
        self.model_processes.ExecuteBeforeOutputStep()

        for process in self._GetListOfProcesses():
            process.ExecuteBeforeOutputStep()

        # write output results GiD: (frequency writing is controlled internally)
        self.GraphicalOutputPrintOutput()

        # processes to be executed after witting the output
        self.model_processes.ExecuteAfterOutputStep()

        for process in self._GetListOfProcesses():
            process.ExecuteAfterOutputStep()

        self.StopTimeMeasuring(self.clock_time,"Finalize Step" , self.report);

    def Finalize(self):

        # Ending the problem (time integration finished)
        self.GraphicalOutputExecuteFinalize()

        self.model_processes.ExecuteFinalize()

        for process in self._GetListOfProcesses():
            process.ExecuteFinalize()

        print("::[KPFEM Simulation]:: Analysis -END- ")
        print(" ")

        # Check solving information for any problem
        #~ self.solver.InfoCheck() # InfoCheck not implemented yet.

        #### END SOLUTION ####

        # Measure process time
        tfp = timer.clock()
        # Measure wall time
        tfw = timer.time()

        print("::[KPFEM Simulation]:: [Elapsed Time = %.2f" % (tfw - self.t0w),"seconds] (%.2f" % (tfp - self.t0p),"seconds of cpu/s time)")

        print(timer.ctime())

        # to create a benchmark: add standard benchmark files and decomment next two lines
        # rename the file to: run_test.py
        #from run_test_benchmark_results import *
        #WriteBenchmarkResults(model_part)


    def SetGraphicalOutput(self):
        if( self.project_parameters.Has("output_configuration") ):
            from pfem_fluid_gid_output_process import GiDOutputProcess
            self.output_settings = self.project_parameters["output_configuration"]
            self.post_process_model_part = self.model.CreateModelPart("output_model_part")
            #KratosMultiphysics.PfemFluidDynamicsApplication.PostProcessUtilities().RebuildPostProcessModelPart(self.post_process_model_part, self.main_model_part)

            return GiDOutputProcess(self.post_process_model_part,
                                    self.problem_name,
                                    self.output_settings)
        else:
            return (KratosMultiphysics.Process())

    def GraphicalOutputExecuteInitialize(self):
        self.graphical_output.ExecuteInitialize()

    def GraphicalOutputExecuteBeforeSolutionLoop(self):
        # writing a initial state results file or single file (if no restart)
        if((self.main_model_part.ProcessInfo).Has(KratosMultiphysics.IS_RESTARTED)):
            if(self.main_model_part.ProcessInfo[KratosMultiphysics.IS_RESTARTED] == False):
                self.graphical_output.ExecuteBeforeSolutionLoop()

    def GraphicalOutputExecuteInitializeSolutionStep(self):
        self.graphical_output.ExecuteInitializeSolutionStep()

    def GraphicalOutputExecuteFinalizeSolutionStep(self):
        self.graphical_output.ExecuteFinalizeSolutionStep()

    def GraphicalOutputPrintOutput(self):
        if( self.project_parameters.Has("output_configuration") ):
            self.post_process_model_part.ProcessInfo[KratosMultiphysics.TIME] = self.main_model_part.ProcessInfo[KratosMultiphysics.TIME]
            if(self.graphical_output.IsOutputStep()):
                KratosMultiphysics.PfemFluidDynamicsApplication.PostProcessUtilities().RebuildPostProcessModelPart(self.post_process_model_part, self.main_model_part)
                print("")
                print("**********************************************************")
                print("---> Print Output at [STEP:",self.step," TIME:",self.time," DT:",self.delta_time,"]")
                print("**********************************************************")
                print("")
                self.graphical_output.PrintOutput()

    def GraphicalOutputExecuteFinalize(self):
        self.graphical_output.ExecuteFinalize()

    def SetParallelSize(self, num_threads):
        parallel = KratosMultiphysics.OpenMPUtils()
        parallel.SetNumThreads(int(num_threads))

    def GetParallelSize(self):
        parallel = KratosMultiphysics.OpenMPUtils()
        return parallel.GetNumThreads()

    def StartTimeMeasuring(self):
        # Measure process time
        time_ip = timer.clock()
        return time_ip

    def StopTimeMeasuring(self, time_ip, process, report):
        # Measure process time
        time_fp = timer.clock()
        if( report ):
            used_time = time_fp - time_ip
            print("::[PFEM Simulation]:: [ %.2f" % round(used_time,2),"s", process," ] ")

    def _GetOrderOfProcessesInitialization(self):
        return ["constraints_process_list",
                "loads_process_list",
                "auxiliar_process_list"]

    #### Main internal methods ####

    def _import_project_parameters(self, input_file):
        import input_manager
        self.input_manager = input_manager.InputManager(input_file)
        return self.input_manager.Getparameters()


if __name__ == "__main__":

    parameter_file_name = "ProjectParameters.json"

    with open(parameter_file_name,'r') as parameter_file:
        parameters = Kratos.Parameters(parameter_file.read())

    model = KratosMultiphysics.Model()
    simulation = PfemFluidDynamicsAnalysis(model,parameters)
    simulation.Run()