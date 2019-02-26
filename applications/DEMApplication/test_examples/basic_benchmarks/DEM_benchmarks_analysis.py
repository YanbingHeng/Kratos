from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
import time as timer
import os
import sys

# Kratos
from KratosMultiphysics import *
from KratosMultiphysics.DEMApplication import *

from DEM_analysis_stage import DEMAnalysisStage
import plot_variables                # Related to benchmarks in Chung, Ooi
import DEM_benchmarks_class as DBC

sys.path.insert(0,'')
start = timer.time()
benchmark_number = int(sys.argv[1])
benchmark = getattr(DBC, 'Benchmark' + str(benchmark_number))()

listDISCONT   = list(range(1,12))
listROLLFR    = list(range(12,13))
listDEMFEM    = list(range(13,18))
listCONT      = list(range(20,27))
listDISclZHAO = [30,32]
listDISclRK   = [31,33]
listGeneric   = [40]

def GetInputParameters():
    file_name = None
    if benchmark_number in listDISCONT:
        file_name = "ProjectParametersDISCONT.json"
    elif benchmark_number in listROLLFR:
        file_name = "ProjectParametersROLLFR.json"
    elif benchmark_number in listDEMFEM:
        file_name = "ProjectParametersDEMFEM.json"
    elif benchmark_number in listCONT:
        file_name = "ProjectParametersDEMCONT.json"
    elif benchmark_number == 27:
        file_name = "ProjectParametersUCS.json"
    elif benchmark_number in listDISclZHAO:
        file_name = "ProjectParametersDISclZHAO.json"
    elif benchmark_number in listDISclRK:
        file_name = "ProjectParametersDISclRK.json"
    elif benchmark_number in listGeneric:
            file_name = "ProjectParametersDEMGeneric.json"
    else:
        Logger.PrintInfo("DEM",'Benchmark number does not exist')
        sys.exit()

    with open(file_name, 'r') as parameters_file:
        parameters = Parameters(parameters_file.read())

    return parameters

class DEMBenchamarksAnalysisStage(DEMAnalysisStage):

    def __init__(self, model, DEM_parameters):
        self.DEM_parameters = DEM_parameters
        self.project_parameters = DEM_parameters
        default_input_parameters = self.GetDefaultInputParameters()
        self.DEM_parameters.ValidateAndAssignDefaults(default_input_parameters)
        self.FixParametersInconsistencies()
        self.main_path = os.getcwd()
        self.nodeplotter = False
        if benchmark_number in listDISCONT:
            self.nodeplotter = True

        super(DEMBenchamarksAnalysisStage, self).__init__(model, DEM_parameters)

    def model_part_reader(self, modelpart, nodeid=0, elemid=0, condid=0):
        return ModelPartIO(modelpart)

    def SetDt(self):
        self._solver.dt = dt

    def Initialize(self):
        self.DEM_parameters["problem_name"].SetString('benchmark' + str(benchmark_number))
        #self.end_time = slt.end_time
        #self.dt = slt.dt
        #self.graph_print_interval = slt.graph_print_interval
        super(DEMBenchamarksAnalysisStage, self).Initialize()

        Logger.PrintInfo("DEM","Computing points in the curve...", 1 + self.number_of_points_in_the_graphic - self.iteration, "point(s) left to finish....",'\n')
        list_of_nodes_ids = [1]
        if self.nodeplotter:
            os.chdir(self.main_path)
            self.plotter = plot_variables.variable_plotter(self.spheres_model_part, list_of_nodes_ids, benchmark_number)
            self.tang_plotter = plot_variables.tangential_force_plotter(self.spheres_model_part, list_of_nodes_ids, self.iteration)

    def ReadModelParts(self):
        super(DEMBenchamarksAnalysisStage, self).ReadModelParts()
        benchmark.set_initial_data(self.spheres_model_part, self.rigid_face_model_part, self.iteration, self.number_of_points_in_the_graphic, coeff_of_restitution_iteration)

    def GetMpFilename(self):
        return 'benchmark' + str(benchmark_number) + "DEM"

    def GetInletFilename(self):
        if benchmark_number == 40:
            return 'benchmark' + str(benchmark_number) + "DEM_Inlet"
        else:
            return 'benchmarkDEM_Inlet'

    def GetFemFilename(self):
        return 'benchmark' + str(benchmark_number) + "DEM_FEM_boundary"

    def GetClusterFilename(self):
        return 'benchmark' + str(benchmark_number) + "DEM_Clusters"

    def GetProblemTypeFilename(self):
        return 'benchmark' + str(benchmark_number)

    def _BeforeSolveOperations(self, time):
        super(DEMBenchamarksAnalysisStage, self)._BeforeSolveOperations(time)
        benchmark.ApplyNodalRotation(time, self._solver.dt, self.spheres_model_part)

    def BeforePrintingOperations(self, time):
        super(DEMBenchamarksAnalysisStage, self).BeforePrintingOperations(time)
        self.SetDt()
        benchmark.generate_graph_points(self.spheres_model_part, self.rigid_face_model_part, self.cluster_model_part, time, self.graph_print_interval, self._solver.dt)

    def Finalize(self):
        benchmark.get_final_data(self.spheres_model_part, self.rigid_face_model_part, self.cluster_model_part)
        if self.nodeplotter:
            os.chdir(self.main_path)
            self.plotter.close_files()
            self.tang_plotter.close_files()

        self.procedures.RemoveFoldersWithResults(self.main_path, self.problem_name)
        super(DEMBenchamarksAnalysisStage, self).Finalize()

    def FinalizeTimeStep(self, time):
        super(DEMBenchamarksAnalysisStage, self).FinalizeTimeStep(time)
        if self.nodeplotter:
            os.chdir(self.main_path)
            self.plotter.plot_variables(time) #Related to the benchmark in Chung, Ooi
            self.tang_plotter.plot_tangential_force(time)

    def CleanUpOperations(self):
        Logger.PrintInfo("DEM","running CleanUpOperations")
        super(DEMBenchamarksAnalysisStage, self).CleanUpOperations()


end_time, dt, graph_print_interval, number_of_points_in_the_graphic, number_of_coeffs_of_restitution = DBC.initialize_time_parameters(benchmark_number)
for coeff_of_restitution_iteration in range(1, number_of_coeffs_of_restitution + 1):
    for iteration in range(1, number_of_points_in_the_graphic + 1):
        model = Model()
        parameters = GetInputParameters()
        slt = DEMBenchamarksAnalysisStage(model, parameters)
        slt.iteration = iteration
        slt.dt = dt
        slt.graph_print_interval = graph_print_interval
        slt.number_of_points_in_the_graphic = number_of_points_in_the_graphic
        slt.number_of_coeffs_of_restitution = number_of_coeffs_of_restitution
        slt.DEM_parameters["FinalTime"].SetDouble(end_time)
        slt.project_parameters["problem_data"]["end_time"].SetDouble(end_time)
        slt.DEM_parameters["MaxTimeStep"].SetDouble(dt)
        slt.Run()
        del slt
    end = timer.time()
    benchmark.print_results(number_of_points_in_the_graphic, dt, elapsed_time = end - start)
#DBC.delete_archives() #.......Removing some unuseful files
