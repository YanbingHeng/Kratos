from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

from KratosMultiphysics import *
from KratosMultiphysics.MappingApplication import *
import Mapper

import os
import process_factory
import KratosMultiphysics.KratosUnittest as KratosUnittest

from random import uniform # needed to set up test (with random values)
from numpy import size

class KratosExecuteNearestNeighborMapperTest(KratosUnittest.TestCase):

    def __init__(self, gid_output):
        self.GiD_output = gid_output

        # Mdpa Input files
        input_file_origin = "KratosExecuteNearestNeighborMapperTest_mdpa/origin"
        input_file_destination     = "KratosExecuteNearestNeighborMapperTest_mdpa/destination"

        parameter_file = open("KratosExecuteNearestNeighborMapperTest_mdpa/KratosExecuteNearestNeighborMapperTest.json",'r')
        ProjectParameters = Parameters(parameter_file.read())

        variable_list = [PRESSURE, VELOCITY]
        self.model_part_origin  = self.ReadModelPartSerial("ModelPartNameOrigin", input_file_origin, variable_list)
        self.model_part_destination = self.ReadModelPartSerial("ModelPartNameDestination", input_file_destination, variable_list)

        # needed for the tests only, usually one does not need to get the submodel-parts for the mapper
        self.interface_sub_model_part_origin = self.model_part_origin.GetSubModelPart("FluidNoSlipInterface3D_interface_orig_fluid")
        self.interface_sub_model_part_destination = self.model_part_destination.GetSubModelPart("StructureInterface3D_interface_dest_struct")

        # Initialize Mapper
        self.nearestNeighborMapper = Mapper.NonMatchingGridMapper(self.model_part_origin, self.model_part_destination, ProjectParameters)

        if (self.GiD_output):
            self.InitializeGiD()

        # self.PrintValuesToPrescribe()

        self.SetPrescribedValues()


    def TestMapConstantScalarValues(self, output_time):
        map_value = 5.123
        variable_origin = PRESSURE
        variable_destination = PRESSURE

        self.SetValuesOnNodes(self.model_part_origin, variable_origin, map_value)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time)

        # Overwriting Values
        self.nearestNeighborMapper.Map(variable_origin, variable_destination)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time)

        self.CheckValues(self.interface_sub_model_part_destination, variable_destination, map_value)

        # Adding Values
        self.nearestNeighborMapper.Map(variable_origin, variable_destination, True)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time + 0.1)

        self.CheckValues(self.interface_sub_model_part_destination, variable_destination, map_value*2)

    def TestInverseMapConstantScalarValues(self, output_time):
        map_value = -8.6647
        variable_origin = PRESSURE
        variable_destination = PRESSURE

        self.SetValuesOnNodes(self.model_part_destination, variable_destination, map_value)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time)

        # Overwriting Values
        self.nearestNeighborMapper.InverseMap(variable_origin, variable_destination)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time)

        self.CheckValues(self.interface_sub_model_part_origin, variable_origin, map_value)

        # Adding Values
        self.nearestNeighborMapper.InverseMap(variable_origin, variable_destination, True)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time + 0.1)

        self.CheckValues(self.interface_sub_model_part_origin, variable_origin, map_value*2)

    def TestMapConstantVectorValues(self, output_time):
        map_value = [15.99, -2.88, 3.123]
        variable_origin = VELOCITY
        variable_destination = VELOCITY

        self.SetValuesOnNodes(self.model_part_origin, variable_origin, map_value)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time)

        # Overwriting Values
        self.nearestNeighborMapper.Map(variable_origin, variable_destination)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time)

        self.CheckValues(self.interface_sub_model_part_destination, variable_destination, map_value)

        # Adding Values
        self.nearestNeighborMapper.Map(variable_origin, variable_destination, True)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time + 0.1)

        self.CheckValues(self.interface_sub_model_part_destination, VELOCITY, [2*x for x in map_value])

    def TestInverseMapConstantVectorValues(self, output_time):
        map_value = [1.4785, -0.88, -33.123]
        variable_origin = VELOCITY
        variable_destination = VELOCITY

        self.SetValuesOnNodes(self.model_part_destination, variable_destination, map_value)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time)

        # Overwriting Values
        self.nearestNeighborMapper.InverseMap(variable_origin, variable_destination)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time)

        self.CheckValues(self.interface_sub_model_part_origin, variable_origin, map_value)

        # Adding Values
        self.nearestNeighborMapper.InverseMap(variable_origin, variable_destination, True)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time + 0.1)

        self.CheckValues(self.interface_sub_model_part_origin, VELOCITY, [2*x for x in map_value])


    def TestMapNonConstantScalarValues(self, output_time):
        variable_origin = PRESSURE
        variable_destination = PRESSURE

        self.SetValuesOnNodesPrescribed(self.interface_sub_model_part_origin, variable_origin, self.scalar_values_origin_send)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time)

        self.nearestNeighborMapper.Map(variable_origin, variable_destination)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time)

        # self.PrintMappedValues(self.interface_sub_model_part_destination, variable_destination) # needed to set up test
        self.CheckValuesPrescribed(self.interface_sub_model_part_destination, variable_destination, self.scalar_values_destination_receive)

    def TestInverseMapNonConstantScalarValues(self, output_time):
        variable_origin = PRESSURE
        variable_destination = PRESSURE

        self.SetValuesOnNodesPrescribed(self.interface_sub_model_part_destination, variable_destination, self.scalar_values_destination_send)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time)

        self.nearestNeighborMapper.InverseMap(variable_origin, variable_destination)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time)

        # self.PrintMappedValues(self.interface_sub_model_part_origin, variable_origin) # needed to set up test
        self.CheckValuesPrescribed(self.interface_sub_model_part_origin, variable_origin, self.scalar_values_origin_receive)

    def TestMapNonConstantVectorValues(self, output_time):
        variable_origin = VELOCITY
        variable_destination = VELOCITY

        self.SetValuesOnNodesPrescribed(self.interface_sub_model_part_origin, VELOCITY, self.vector_values_origin_send)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time)

        self.nearestNeighborMapper.Map(variable_origin, variable_destination)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time)

        # self.PrintMappedValues(self.interface_sub_model_part_destination, variable_destination) # needed to set up test
        self.CheckValuesPrescribed(self.interface_sub_model_part_destination, variable_destination, self.vector_values_destination_receive)

    def TestInverseMapNonConstantVectorValues(self, output_time):
        variable_origin = VELOCITY
        variable_destination = VELOCITY

        self.SetValuesOnNodesPrescribed(self.interface_sub_model_part_destination, VELOCITY, self.vector_values_destination_send)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_destination, self.model_part_destination, variable_destination, output_time)

        self.nearestNeighborMapper.InverseMap(variable_origin, variable_destination)

        if (self.GiD_output):
            self.WriteNodalResultsCustom(self.gid_io_origin, self.model_part_origin, variable_origin, output_time)

        # self.PrintMappedValues(self.interface_sub_model_part_origin, variable_origin) # needed to set up test
        self.CheckValuesPrescribed(self.interface_sub_model_part_origin, variable_origin, self.vector_values_origin_receive)


    def ReadModelPartSerial(self, model_part_name, model_part_input_file, variable_list):
        model_part = ModelPart(model_part_name)
        for variable in variable_list:
            model_part.AddNodalSolutionStepVariable(variable)

        model_part_io = ModelPartIO(model_part_input_file)
        model_part_io.ReadModelPart(model_part)

        model_part.ProcessInfo.SetValue(DOMAIN_SIZE, 3)
        model_part.SetBufferSize(1)

        return model_part

    def SetValuesOnNodes(self, model_part, variable, value):
        for node in model_part.Nodes:
            node.SetSolutionStepValue(variable, value)

    def SetValuesOnNodesPrescribed(self, model_part, variable, nodal_values):
        for node in model_part.Nodes:
            nodal_coords = (node.X, node.Y, node.Z)
            node.SetSolutionStepValue(variable, nodal_values[nodal_coords])

    def CheckValues(self, model_part, variable, value_mapped):
        for node in model_part.Nodes:
            value_expected = node.GetSolutionStepValue(variable)
            if (size(value_mapped) > 1): # Variable is a vector
                self.assertAlmostEqualVector(value_mapped,value_expected)
            else: # Variable is a scalar
                self.assertAlmostEqual(value_mapped,value_expected,4)

    def CheckValuesPrescribed(self, model_part, variable, nodal_values):
        for node in model_part.Nodes:
            value_mapped = node.GetSolutionStepValue(variable)
            nodal_coords = (node.X, node.Y, node.Z)
            value_expected = nodal_values[nodal_coords]
            if (size(value_mapped) > 1): # Variable is a vector
                self.assertAlmostEqualVector(value_mapped,value_expected)
            else: # Variable is a scalar
                self.assertAlmostEqual(value_mapped,value_expected,4)

    def assertAlmostEqualVector(self, values_mapped, values_expected):
        for i in range(0,3):
            self.assertAlmostEqual(values_mapped[i],values_expected[i],4)


    def InitializeGiD(self):
        # Initialize GidIO
        output_file_origin = "KratosExecuteNearestNeighborMapperTest_gid_output/output_origin"
        output_file_destination = "KratosExecuteNearestNeighborMapperTest_gid_output/output_destination"

        gid_mode = GiDPostMode.GiD_PostAscii
        multifile = MultiFileFlag.MultipleFiles
        deformed_mesh_flag = WriteDeformedMeshFlag.WriteUndeformed
        write_conditions = WriteConditionsFlag.WriteConditions

        self.gid_io_origin = GidIO(output_file_origin, gid_mode, multifile, deformed_mesh_flag, write_conditions)
        self.gid_io_destination  = GidIO(output_file_destination,  gid_mode, multifile, deformed_mesh_flag, write_conditions)

        # Initialize Results Output
        self.gid_io_origin.InitializeResults(0, self.model_part_origin.GetMesh())
        self.gid_io_destination.InitializeResults( 0, self.model_part_destination.GetMesh())

        # Print original meshes
        self.write_mesh(self.model_part_origin, self.gid_io_origin)
        self.write_mesh(self.model_part_destination, self.gid_io_destination)

    def WriteNodalResultsCustom(self, gid_io, model_part, variable, output_time):
        gid_io.WriteNodalResults(variable, model_part.Nodes, output_time, 0)

    ### Function to write meshes ###
    def write_mesh(self, model_part, gid_io):
        gid_io.InitializeMesh(0)
        gid_io.WriteMesh(model_part.GetMesh())
        gid_io.FinalizeMesh()

    def PrintValuesToPrescribe(self):
        values_range = [-100, 100]
        values_precision = 4
        print("Origin ModelPart; Scalar Values")
        for node in self.interface_sub_model_part_origin.Nodes:
            nodal_coords = (node.X, node.Y, node.Z)
            value = round(uniform(values_range[0],values_range[1]),values_precision)
            print(str(nodal_coords) + " : " + str(value) + ",")

        print("\n\nDestination ModelPart; Scalar Values")
        for node in self.interface_sub_model_part_destination.Nodes:
            nodal_coords = (node.X, node.Y, node.Z)
            value = round(uniform(values_range[0],values_range[1]),values_precision)
            print(str(nodal_coords) + " : " + str(value) + ",")

        print("\n\nOrigin ModelPart; Vector Values")
        for node in self.interface_sub_model_part_origin.Nodes:
            nodal_coords = (node.X, node.Y, node.Z)
            value_x = round(uniform(values_range[0],values_range[1]),values_precision)
            value_y = round(uniform(values_range[0],values_range[1]),values_precision)
            value_z = round(uniform(values_range[0],values_range[1]),values_precision)
            values = (value_x, value_y, value_z)
            print(str(nodal_coords) + " : " + str(values) + ",")

        print("\n\nDestination ModelPart; Vector Values")
        for node in self.interface_sub_model_part_destination.Nodes:
            nodal_coords = (node.X, node.Y, node.Z)
            value_x = round(uniform(values_range[0],values_range[1]),values_precision)
            value_y = round(uniform(values_range[0],values_range[1]),values_precision)
            value_z = round(uniform(values_range[0],values_range[1]),values_precision)
            values = (value_x, value_y, value_z)
            print(str(nodal_coords) + " : " + str(values) + ",")

        err # needed to get the output

    def PrintMappedValues(self, model_part, variable):
        for node in model_part.Nodes:
            nodal_coords = (node.X, node.Y, node.Z)
            print(str(nodal_coords) + " : " + str(node.GetSolutionStepValue(variable)) + ",")
        err # needed to get the output

    def SetPrescribedValues(self):
        self.scalar_values_origin_send = {
        (-0.5, -0.5, 0.0) : 74.8612,
        (-0.3002, -0.5, 0.0) : 71.2017,
        (-0.5, -0.364, 0.0) : -80.2386,
        (-0.32243, -0.21657, 0.0) : 8.5714,
        (-0.199, -0.5, 0.0) : -81.6182,
        (-0.15359, -0.33, 0.0) : -85.2777,
        (-0.5, -0.17, 0.0) : -94.3847,
        (-0.15359, -0.1004, 0.0) : 70.1815,
        (-0.32679, -0.00077, 0.0) : -95.3669,
        (0.01401, -0.35733, 0.0) : 41.267,
        (0.173, -0.5, 0.0) : 77.2532,
        (-0.5, 0.188, 0.0) : -58.9351,
        (-0.15359, 0.1888, 0.0) : -75.2223,
        (0.18145, -0.31476, 0.0) : -1.8728,
        (0.01262, -0.04, 0.0) : -88.0393,
        (-0.32293, 0.23667, 0.0) : -0.6181,
        (0.19212, -0.177, 0.0) : 99.6869,
        (-0.5, 0.355, 0.0) : -7.3811,
        (0.3568, -0.5, 0.0) : 29.8251,
        (-0.15559, 0.355, 0.0) : -69.2206,
        (0.33541, -0.21888, 0.0) : 57.3482,
        (0.03398, 0.21551, 0.0) : 61.6527,
        (0.19244, 0.1066, 0.0) : 88.1457,
        (0.32613, 0.012, 0.0) : 92.5446,
        (0.5, -0.5, 0.0) : 46.5348,
        (-0.5, 0.5, 0.0) : -65.896,
        (-0.31004, 0.5, 0.0) : -72.4578,
        (0.5, -0.325, 0.0) : 65.5213,
        (0.19885, 0.29804, 0.0) : -79.2115,
        (0.34366, 0.15961, 0.0) : 0.3687,
        (-0.10041, 0.5, 0.0) : 0.5273,
        (0.5, -0.10561, 0.0) : 17.4257,
        (0.152, 0.5, 0.0) : 67.2235,
        (0.5, 0.19874, 0.0) : -91.6093,
        (0.3321, 0.5, 0.0) : -70.3876,
        (0.5, 0.30456, 0.0) : 87.3811,
        (0.5, 0.5, 0.0) : 47.4853 }

        self.scalar_values_destination_send = {
        (-0.5, -0.5, 0.0) : -29.2379,
        (-0.5, -0.246, 0.0) : -98.8008,
        (-0.262, -0.5, 0.0) : -21.5213,
        (-0.28857, -0.14832, 0.0) : 78.5838,
        (-0.5, 0.06104, 0.0) : 86.4897,
        (0.0553, -0.5, 0.0) : 33.4003,
        (-0.04361, -0.28681, 0.0) : 60.6458,
        (-0.08274, -0.01745, 0.0) : 44.3008,
        (-0.27185, 0.1518, 0.0) : -14.2951,
        (0.15858, -0.32561, 0.0) : 45.8605,
        (-0.5, 0.2592, 0.0) : -38.8276,
        (0.2511, -0.5, 0.0) : 20.6577,
        (0.19743, -0.1191, 0.0) : -70.3052,
        (-0.14373, 0.25658, 0.0) : 11.9008,
        (0.19981, 0.11831, 0.0) : -10.9708,
        (0.5, -0.5, 0.0) : -70.6218,
        (-0.5, 0.5, 0.0) : -71.0203,
        (0.5, -0.2425, 0.0) : 57.4228,
        (-0.25129, 0.5, 0.0) : -99.7596,
        (0.11949, 0.31894, 0.0) : 21.5272,
        (0.5, 0.0052, 0.0) : -22.0285,
        (0.0124, 0.5, 0.0) : 35.9431,
        (0.5, 0.249, 0.0) : 87.495,
        (0.25186, 0.5, 0.0) : 93.018,
        (0.5, 0.5, 0.0) : 13.4472 }

        self.vector_values_origin_send = {
        (-0.5, -0.5, 0.0) : (-82.6987, 95.4827, 89.3428),
        (-0.3002, -0.5, 0.0) : (60.7885, 89.6246, -15.217),
        (-0.5, -0.364, 0.0) : (60.5731, -87.3619, 14.2162),
        (-0.32243, -0.21657, 0.0) : (94.3739, -47.977, 84.8945),
        (-0.199, -0.5, 0.0) : (-45.466, 10.6292, 13.0954),
        (-0.15359, -0.33, 0.0) : (70.8957, -79.9669, 10.832),
        (-0.5, -0.17, 0.0) : (97.4695, 81.8736, -88.5944),
        (-0.15359, -0.1004, 0.0) : (-94.7962, 70.3255, 40.8216),
        (-0.32679, -0.00077, 0.0) : (-86.3453, 30.0437, -63.3355),
        (0.01401, -0.35733, 0.0) : (81.9431, 44.044, -76.4111),
        (0.173, -0.5, 0.0) : (-80.3916, -32.4842, 67.057),
        (-0.5, 0.188, 0.0) : (-46.661, 23.6675, 89.1151),
        (-0.15359, 0.1888, 0.0) : (47.3517, 34.2461, -56.1094),
        (0.18145, -0.31476, 0.0) : (68.6203, 49.2641, 2.576),
        (0.01262, -0.04, 0.0) : (-95.6574, 80.6006, -20.3202),
        (-0.32293, 0.23667, 0.0) : (35.495, -89.1297, 72.2261),
        (0.19212, -0.177, 0.0) : (70.5812, -40.4829, 97.703),
        (-0.5, 0.355, 0.0) : (-26.6771, -45.2197, -43.927),
        (0.3568, -0.5, 0.0) : (-33.4444, -59.0702, -23.3442),
        (-0.15559, 0.355, 0.0) : (-62.3037, -60.0014, 11.5194),
        (0.33541, -0.21888, 0.0) : (-36.7822, -74.8982, 94.9569),
        (0.03398, 0.21551, 0.0) : (87.1369, 59.4913, -78.8707),
        (0.19244, 0.1066, 0.0) : (-31.8417, -14.0344, -9.4779),
        (0.32613, 0.012, 0.0) : (-58.5903, 63.2413, 80.5193),
        (0.5, -0.5, 0.0) : (-43.5106, -57.763, 98.5749),
        (-0.5, 0.5, 0.0) : (-30.8904, 50.4188, 89.1966),
        (-0.31004, 0.5, 0.0) : (93.0028, -53.8615, -34.9477),
        (0.5, -0.325, 0.0) : (0.9265, -40.9413, -36.5583),
        (0.19885, 0.29804, 0.0) : (70.0601, 63.2991, -11.9064),
        (0.34366, 0.15961, 0.0) : (-28.9694, -26.4793, -6.908),
        (-0.10041, 0.5, 0.0) : (83.3125, -14.6739, 23.429),
        (0.5, -0.10561, 0.0) : (63.7904, -21.4233, 83.3817),
        (0.152, 0.5, 0.0) : (77.2986, -28.2953, 35.1405),
        (0.5, 0.19874, 0.0) : (20.9458, -28.2064, -7.7992),
        (0.3321, 0.5, 0.0) : (12.9683, 66.6925, 28.39),
        (0.5, 0.30456, 0.0) : (78.6603, -82.0786, -0.9975),
        (0.5, 0.5, 0.0) : (17.363, -14.019, 98.6192) }

        self.vector_values_destination_send = {
        (-0.5, -0.5, 0.0) : (-56.4463, -25.9128, 65.2663),
        (-0.5, -0.246, 0.0) : (15.5472, -50.5599, -6.8589),
        (-0.262, -0.5, 0.0) : (-42.618, -77.049, 15.075),
        (-0.28857, -0.14832, 0.0) : (-45.3971, 12.1528, -76.4542),
        (-0.5, 0.06104, 0.0) : (76.7786, -77.7307, 82.6615),
        (0.0553, -0.5, 0.0) : (-75.3713, 59.8855, -26.5626),
        (-0.04361, -0.28681, 0.0) : (80.9893, -54.946, 9.0543),
        (-0.08274, -0.01745, 0.0) : (72.7381, -47.1258, 11.7604),
        (-0.27185, 0.1518, 0.0) : (52.1867, -89.2781, -25.5366),
        (0.15858, -0.32561, 0.0) : (6.3482, -88.3025, -67.2723),
        (-0.5, 0.2592, 0.0) : (73.4525, 56.9959, -57.0295),
        (0.2511, -0.5, 0.0) : (-30.4067, 83.2038, 82.9062),
        (0.19743, -0.1191, 0.0) : (-95.7395, 6.1749, 29.4679),
        (-0.14373, 0.25658, 0.0) : (-44.646, -11.6643, 36.3388),
        (0.19981, 0.11831, 0.0) : (8.9531, 48.6582, 22.0265),
        (0.5, -0.5, 0.0) : (-63.2004, -89.0804, -47.276),
        (-0.5, 0.5, 0.0) : (-62.2399, 78.8062, -43.6925),
        (0.5, -0.2425, 0.0) : (-61.2615, -96.1305, 62.5688),
        (-0.25129, 0.5, 0.0) : (-52.5028, 7.931, 66.3195),
        (0.11949, 0.31894, 0.0) : (-43.234, -97.6892, -38.1162),
        (0.5, 0.0052, 0.0) : (-9.6424, 3.8129, -3.9237),
        (0.0124, 0.5, 0.0) : (99.2372, 38.2263, 92.0563),
        (0.5, 0.249, 0.0) : (-6.5423, 16.3678, 0.9326),
        (0.25186, 0.5, 0.0) : (-30.2543, 19.2205, 10.2341),
        (0.5, 0.5, 0.0) : (91.3727, -50.0157, -27.6178) }


        self.scalar_values_origin_receive = {
        (-0.5, -0.5, 0.0) : -29.2379,
        (-0.3002, -0.5, 0.0) : -21.5213,
        (-0.5, -0.364, 0.0) : -98.8008,
        (-0.32243, -0.21657, 0.0) : 78.5838,
        (-0.199, -0.5, 0.0) : -21.5213,
        (-0.15359, -0.33, 0.0) : 60.6458,
        (-0.5, -0.17, 0.0) : -98.8008,
        (-0.15359, -0.1004, 0.0) : 44.3008,
        (-0.32679, -0.00077, 0.0) : 78.5838,
        (0.01401, -0.35733, 0.0) : 60.6458,
        (0.173, -0.5, 0.0) : 20.6577,
        (-0.5, 0.188, 0.0) : -38.8276,
        (-0.15359, 0.1888, 0.0) : 11.9008,
        (0.18145, -0.31476, 0.0) : 45.8605,
        (0.01262, -0.04, 0.0) : 44.3008,
        (-0.32293, 0.23667, 0.0) : -14.2951,
        (0.19212, -0.177, 0.0) : -70.3052,
        (-0.5, 0.355, 0.0) : -38.8276,
        (0.3568, -0.5, 0.0) : 20.6577,
        (-0.15559, 0.355, 0.0) : 11.9008,
        (0.33541, -0.21888, 0.0) : 57.4228,
        (0.03398, 0.21551, 0.0) : 21.5272,
        (0.19244, 0.1066, 0.0) : -10.9708,
        (0.32613, 0.012, 0.0) : -10.9708,
        (0.5, -0.5, 0.0) : -70.6218,
        (-0.5, 0.5, 0.0) : -71.0203,
        (-0.31004, 0.5, 0.0) : -99.7596,
        (0.5, -0.325, 0.0) : 57.4228,
        (0.19885, 0.29804, 0.0) : 21.5272,
        (0.34366, 0.15961, 0.0) : -10.9708,
        (-0.10041, 0.5, 0.0) : 35.9431,
        (0.5, -0.10561, 0.0) : -22.0285,
        (0.152, 0.5, 0.0) : 93.018,
        (0.5, 0.19874, 0.0) : 87.495,
        (0.3321, 0.5, 0.0) : 93.018,
        (0.5, 0.30456, 0.0) : 87.495,
        (0.5, 0.5, 0.0) : 13.4472 }

        self.scalar_values_destination_receive = {
        (-0.5, -0.5, 0.0) : 74.8612,
        (-0.5, -0.246, 0.0) : -94.3847,
        (-0.262, -0.5, 0.0) : 71.2017,
        (-0.28857, -0.14832, 0.0) : 8.5714,
        (-0.5, 0.06104, 0.0) : -58.9351,
        (0.0553, -0.5, 0.0) : 77.2532,
        (-0.04361, -0.28681, 0.0) : 41.267,
        (-0.08274, -0.01745, 0.0) : -88.0393,
        (-0.27185, 0.1518, 0.0) : -0.6181,
        (0.15858, -0.32561, 0.0) : -1.8728,
        (-0.5, 0.2592, 0.0) : -58.9351,
        (0.2511, -0.5, 0.0) : 77.2532,
        (0.19743, -0.1191, 0.0) : 99.6869,
        (-0.14373, 0.25658, 0.0) : -75.2223,
        (0.19981, 0.11831, 0.0) : 88.1457,
        (0.5, -0.5, 0.0) : 46.5348,
        (-0.5, 0.5, 0.0) : -65.896,
        (0.5, -0.2425, 0.0) : 65.5213,
        (-0.25129, 0.5, 0.0) : -72.4578,
        (0.11949, 0.31894, 0.0) : -79.2115,
        (0.5, 0.0052, 0.0) : 17.4257,
        (0.0124, 0.5, 0.0) : 0.5273,
        (0.5, 0.249, 0.0) : -91.6093,
        (0.25186, 0.5, 0.0) : -70.3876,
        (0.5, 0.5, 0.0) : 47.4853 }

        self.vector_values_origin_receive = {
        (-0.5, -0.5, 0.0) : (-56.4463,-25.9128,65.2663),
        (-0.3002, -0.5, 0.0) : (-42.618,-77.049,15.075),
        (-0.5, -0.364, 0.0) : (15.5472,-50.5599,-6.8589),
        (-0.32243, -0.21657, 0.0) : (-45.3971,12.1528,-76.4542),
        (-0.199, -0.5, 0.0) : (-42.618,-77.049,15.075),
        (-0.15359, -0.33, 0.0) : (80.9893,-54.946,9.0543),
        (-0.5, -0.17, 0.0) : (15.5472,-50.5599,-6.8589),
        (-0.15359, -0.1004, 0.0) : (72.7381,-47.1258,11.7604),
        (-0.32679, -0.00077, 0.0) : (-45.3971,12.1528,-76.4542),
        (0.01401, -0.35733, 0.0) : (80.9893,-54.946,9.0543),
        (0.173, -0.5, 0.0) : (-30.4067,83.2038,82.9062),
        (-0.5, 0.188, 0.0) : (73.4525,56.9959,-57.0295),
        (-0.15359, 0.1888, 0.0) : (-44.646,-11.6643,36.3388),
        (0.18145, -0.31476, 0.0) : (6.3482,-88.3025,-67.2723),
        (0.01262, -0.04, 0.0) : (72.7381,-47.1258,11.7604),
        (-0.32293, 0.23667, 0.0) : (52.1867,-89.2781,-25.5366),
        (0.19212, -0.177, 0.0) : (-95.7395,6.1749,29.4679),
        (-0.5, 0.355, 0.0) : (73.4525,56.9959,-57.0295),
        (0.3568, -0.5, 0.0) : (-30.4067,83.2038,82.9062),
        (-0.15559, 0.355, 0.0) : (-44.646,-11.6643,36.3388),
        (0.33541, -0.21888, 0.0) : (-61.2615,-96.1305,62.5688),
        (0.03398, 0.21551, 0.0) : (-43.234,-97.6892,-38.1162),
        (0.19244, 0.1066, 0.0) : (8.9531,48.6582,22.0265),
        (0.32613, 0.012, 0.0) : (8.9531,48.6582,22.0265),
        (0.5, -0.5, 0.0) : (-63.2004,-89.0804,-47.276),
        (-0.5, 0.5, 0.0) : (-62.2399,78.8062,-43.6925),
        (-0.31004, 0.5, 0.0) : (-52.5028,7.931,66.3195),
        (0.5, -0.325, 0.0) : (-61.2615,-96.1305,62.5688),
        (0.19885, 0.29804, 0.0) : (-43.234,-97.6892,-38.1162),
        (0.34366, 0.15961, 0.0) : (8.9531,48.6582,22.0265),
        (-0.10041, 0.5, 0.0) : (99.2372,38.2263,92.0563),
        (0.5, -0.10561, 0.0) : (-9.6424,3.8129,-3.9237),
        (0.152, 0.5, 0.0) : (-30.2543,19.2205,10.2341),
        (0.5, 0.19874, 0.0) : (-6.5423,16.3678,0.9326),
        (0.3321, 0.5, 0.0) : (-30.2543,19.2205,10.2341),
        (0.5, 0.30456, 0.0) : (-6.5423,16.3678,0.9326),
        (0.5, 0.5, 0.0) : (91.3727,-50.0157,-27.6178) }

        self.vector_values_destination_receive = {
        (-0.5, -0.5, 0.0) : (-82.6987,95.4827,89.3428),
        (-0.5, -0.246, 0.0) : (97.4695,81.8736,-88.5944),
        (-0.262, -0.5, 0.0) : (60.7885,89.6246,-15.217),
        (-0.28857, -0.14832, 0.0) : (94.3739,-47.977,84.8945),
        (-0.5, 0.06104, 0.0) : (-46.661,23.6675,89.1151),
        (0.0553, -0.5, 0.0) : (-80.3916,-32.4842,67.057),
        (-0.04361, -0.28681, 0.0) : (81.9431,44.044,-76.4111),
        (-0.08274, -0.01745, 0.0) : (-95.6574,80.6006,-20.3202),
        (-0.27185, 0.1518, 0.0) : (35.495,-89.1297,72.2261),
        (0.15858, -0.32561, 0.0) : (68.6203,49.2641,2.576),
        (-0.5, 0.2592, 0.0) : (-46.661,23.6675,89.1151),
        (0.2511, -0.5, 0.0) : (-80.3916,-32.4842,67.057),
        (0.19743, -0.1191, 0.0) : (70.5812,-40.4829,97.703),
        (-0.14373, 0.25658, 0.0) : (47.3517,34.2461,-56.1094),
        (0.19981, 0.11831, 0.0) : (-31.8417,-14.0344,-9.4779),
        (0.5, -0.5, 0.0) : (-43.5106,-57.763,98.5749),
        (-0.5, 0.5, 0.0) : (-30.8904,50.4188,89.1966),
        (0.5, -0.2425, 0.0) : (0.9265,-40.9413,-36.5583),
        (-0.25129, 0.5, 0.0) : (93.0028,-53.8615,-34.9477),
        (0.11949, 0.31894, 0.0) : (70.0601,63.2991,-11.9064),
        (0.5, 0.0052, 0.0) : (63.7904,-21.4233,83.3817),
        (0.0124, 0.5, 0.0) : (83.3125,-14.6739,23.429),
        (0.5, 0.249, 0.0) : (20.9458,-28.2064,-7.7992),
        (0.25186, 0.5, 0.0) : (12.9683,66.6925,28.39),
        (0.5, 0.5, 0.0) : (17.363,-14.019,98.6192) }
