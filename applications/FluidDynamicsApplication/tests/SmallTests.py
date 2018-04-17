import os

# Import Kratos
from KratosMultiphysics import *

try:
    import KratosMultiphysics.ExternalSolversApplication
    have_external_solvers = True
except ImportError as e:
    have_external_solvers = False

# Import KratosUnittest
import KratosMultiphysics.KratosUnittest as KratosUnittest
import KratosExecuteEmbeddedTest as ExecuteEmbeddedTest
# import KratosExecuteManufacturedSolutionTest as ExecuteManufacturedSolutionTest

# This utility will control the execution scope in case we need to access files or we depend
# on specific relative locations of the files.

# TODO: Should we move this to KratosUnittest?
class controlledExecutionScope:
    def __init__(self, scope):
        self.currentPath = os.getcwd()
        self.scope = scope

    def __enter__(self):
        os.chdir(self.scope)

    def __exit__(self, type, value, traceback):
        os.chdir(self.currentPath)


class EmbeddedTestFactory(KratosUnittest.TestCase):

    def setUp(self):
        # Within this location context:
        with controlledExecutionScope(os.path.dirname(os.path.realpath(__file__))):
            # Get the ProjectParameters file
            with open(self.file_name + "_parameters.json", 'r') as parameter_file:
                ProjectParameters = Parameters(parameter_file.read())

            # Create the test
            self.test = ExecuteEmbeddedTest.KratosExecuteEmbeddedTest(ProjectParameters)

    def test_execution(self):
        # Within this location context:
        with controlledExecutionScope(os.path.dirname(os.path.realpath(__file__))):
            self.test.Solve()

    def tearDown(self):
        pass


class EmbeddedArtificialCompressibilityTest(EmbeddedTestFactory):
    file_name = "EmbeddedArtificialCompressibilityTest/EmbeddedArtificialCompressibilityTest"


class NavierStokesWallConditionTest(EmbeddedTestFactory):
    file_name = "NavierStokesWallConditionTest/NavierSokesWallConditionTest"
