/*
//  KRATOS  _____________
//         /  _/ ____/   |
//         / // / __/ /| |
//       _/ // /_/ / ___ |
//      /___/\____/_/  |_| Application
//
//  Main authors:   Thomas Oberbichler
*/

#include "iga_application_variables.h"

namespace Kratos
{

KRATOS_CREATE_VARIABLE(double, NURBS_CONTROL_POINT_WEIGHT)

KRATOS_CREATE_VARIABLE(Vector, COORDINATES)
KRATOS_CREATE_VARIABLE(double, CROSSSECTION_ROTATION)
// KRATOS_CREATE_VARIABLE(Vector, TANGENTS)

KRATOS_CREATE_VARIABLE(double, CROSS_AREA)
KRATOS_CREATE_VARIABLE(double, PRESTRESS_CAUCHY)

KRATOS_CREATE_VARIABLE(Vector, SHAPE_FUNCTION_VALUES)
KRATOS_CREATE_VARIABLE(Vector, SHAPE_FUNCTION_LOCAL_DER_1)
KRATOS_CREATE_VARIABLE(Vector, SHAPE_FUNCTION_LOCAL_DER_2)
KRATOS_CREATE_VARIABLE(Vector, SHAPE_FUNCTION_LOCAL_DER_3)
KRATOS_CREATE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_DERIVATIVES)
KRATOS_CREATE_VARIABLE(Matrix, SHAPE_FUNCTION_LOCAL_SECOND_DERIVATIVES)



KRATOS_CREATE_VARIABLE(double, RAYLEIGH_ALPHA)
KRATOS_CREATE_VARIABLE(double, RAYLEIGH_BETA)

KRATOS_CREATE_VARIABLE(double, DISPLACEMENT_ROTATION)
KRATOS_CREATE_VARIABLE(double, REACTION_ROTATION)

KRATOS_CREATE_VARIABLE(std::string, DEBUG_EXPECTED_DATA)

KRATOS_CREATE_VARIABLE(double, SHEAR_MODULUS)
KRATOS_CREATE_VARIABLE(double, SHEAR_A)
KRATOS_CREATE_VARIABLE(double, SHEAR_B)

KRATOS_CREATE_VARIABLE(double, PHI)
KRATOS_CREATE_VARIABLE(double, PHI_DER_1)
KRATOS_CREATE_VARIABLE(double, PHI_DER_2)

KRATOS_CREATE_VARIABLE(Vector, T0)
KRATOS_CREATE_VARIABLE(Vector, T0_DER)
KRATOS_CREATE_VARIABLE(Vector, N0)

KRATOS_CREATE_VARIABLE(Vector, BASE_A1)
KRATOS_CREATE_VARIABLE(Vector, BASE_A2)
KRATOS_CREATE_VARIABLE(Vector, BASE_A3)
KRATOS_CREATE_VARIABLE(Vector, BASE_A1_1)
KRATOS_CREATE_VARIABLE(Vector, BASE_A2_1)
KRATOS_CREATE_VARIABLE(Vector, BASE_A3_1)

KRATOS_CREATE_VARIABLE(double, MOMENT_OF_INERTIA_Y)
KRATOS_CREATE_VARIABLE(double, MOMENT_OF_INERTIA_Z)
KRATOS_CREATE_VARIABLE(double, MOMENT_OF_INERTIA_T)

KRATOS_CREATE_VARIABLE(Vector, LOAD_VECTOR_MOMENT)
KRATOS_CREATE_VARIABLE(double, LOAD_MOMENT)

KRATOS_CREATE_VARIABLE(int, DIRICHLET_CONDITION_TYPE)
KRATOS_CREATE_VARIABLE(double, PENALTY_DISPLACEMENT_X)
KRATOS_CREATE_VARIABLE(double, PENALTY_DISPLACEMENT_Y)
KRATOS_CREATE_VARIABLE(double, PENALTY_DISPLACEMENT_Z)
KRATOS_CREATE_VARIABLE(double, PENALTY_ROTATION)
KRATOS_CREATE_VARIABLE(double, PENALTY_TORSION)




// KRATOS_CREATE_VARIABLE(double, NORMAL_STRESS)            // already existing in KRATSO kernel 
// KRATOS_CREATE_VARIABLE(Vector, TANGENTIAL_STRESS)
KRATOS_CREATE_VARIABLE(Vector, BENDING_STRESS)



} // namespace Kratos
