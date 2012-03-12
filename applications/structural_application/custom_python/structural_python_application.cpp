/*
==============================================================================
KratosStructuralApplication
A library based on:
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi, Janosch Stascheit, Felix Nagel
pooyan@cimne.upc.edu
rrossi@cimne.upc.edu
janosch.stascheit@rub.de
nagel@sd.rub.de
- CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain
- Ruhr-University Bochum, Institute for Structural Mechanics, Germany


Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNERS.

The  above  copyright  notice  and  this permission  notice  shall  be
included in all copies or substantial portions of the Software.

THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY
CLAIM, DAMAGES OR  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT,
TORT  OR OTHERWISE, ARISING  FROM, OUT  OF OR  IN CONNECTION  WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

==============================================================================
*/

//
//   Project Name:        Kratos
//   Last modified by:    $Author: virginia $
//   Date:                $Date: 2009-01-23 14:39:59 $
//   Revision:            $Revision: 1.17 $
//
//

// System includes

#if defined(KRATOS_PYTHON)
// External includes
#include <boost/python.hpp>


// Project includes
#include "includes/define.h"
#include "structural_application.h"
#include "custom_python/add_custom_strategies_to_python.h"
#include "custom_python/add_constitutive_laws_to_python.h"
#include "custom_python/add_custom_utilities_to_python.h"
#include "custom_python/add_custom_io_to_python.h"
#include "custom_python/add_custom_parallel_strategies_to_python.h"
#include "custom_python/add_conditions_to_python.h"
#include "custom_python/add_fluency_criteria_to_python.h"

namespace Kratos
{

    namespace Python
    {

        using namespace boost::python;



        BOOST_PYTHON_MODULE( KratosStructuralApplication )
        {

            class_ < KratosStructuralApplication,
            KratosStructuralApplication::Pointer,
            bases<KratosApplication>, boost::noncopyable > ( "KratosStructuralApplication" )
            ;

            AddCustomStrategiesToPython();
            AddConstitutiveLawsToPython();
            AddCustomUtilitiesToPython();
            AddCustomIOToPython();
            AddCustomParallelStrategiesToPython();
            AddCustomConditionsToPython();
            AddFluencyCriteriaToPython();

            //registering variables in python
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AREA )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( IX )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( IY )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( IZ )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( MATRIX_A )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( MATRIX_B )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( MATRIX_D )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( COMPOSITE_DIRECTION )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( GEOMETRIC_STIFFNESS )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONTACT_PENETRATION )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( COORDINATES )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( STRESSES )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( FLUID_FLOWS )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( EXCESS_PORE_WATER_PRESSURE )

            //KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(VAUX);
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( WRINKLING_APPROACH )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( GREEN_LAGRANGE_STRAIN_TENSOR )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( PK2_STRESS_TENSOR )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( AUXILIARY_MATRIX_1 )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( YOUNG_MODULUS )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( POISSON_RATIO )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( MU );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ALPHA );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( RETRACTION_TIME );
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( THICKNESS )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( NEGATIVE_FACE_PRESSURE )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE( POSITIVE_FACE_PRESSURE )

            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONSTITUTIVE_LAW )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( INTERNAL_VARIABLES )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DP_EPSILON )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( INSITU_STRESS )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DP_ALPHA1 )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DP_K )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CALCULATE_INSITU_STRESS )
            //CONTACT_LINK_MASTER is defined in condition.h
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_LINK_MASTER );
            //CONTACT_LINK_SLAVE is defined in condition.h
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_LINK_SLAVE );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(MASTER_CONTACT_LOCAL_POINT );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(MASTER_CONTACT_CURRENT_LOCAL_POINT );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(MASTER_CONTACT_LAST_CURRENT_LOCAL_POINT );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(SLAVE_CONTACT_LOCAL_POINT );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(MASTER_CONTACT_GLOBAL_POINT );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(MASTER_CONTACT_CURRENT_GLOBAL_POINT );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(SLAVE_CONTACT_GLOBAL_POINT );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(CONTACT_RAMP );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( PENALTY );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(INITIAL_PENALTY );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(MAXIMUM_PENALTY );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(RAMP_CRITERION );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(RAMP_FACTOR );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( PENALTY_T );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(INITIAL_PENALTY_T );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(MAXIMUM_PENALTY_T );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(RAMP_CRITERION_T );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(RAMP_FACTOR_T );
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE(FRICTION_COEFFICIENT );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( LAMBDA );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( MIU );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( LAMBDAS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( LAMBDAS_T );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( GAPS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DELTA_LAMBDAS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DELTA_LAMBDAS_T );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(MAX_UZAWA_ITERATIONS);
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONTACT_SLAVE_INTEGRATION_POINT_INDEX );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONTACT_LINK_M );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONTACT_DOUBLE_CHECK );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( IS_CONTACT_MASTER );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( IS_CONTACT_SLAVE );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONSTRAINT_MATRIX );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONSTRAINT_VECTOR );



            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(K_CONTACT );
            //  KRATOS_REGISTER_IN_PYTHON_VARIABLE(K_CONTACT_T );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( STICK );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( FIRST_TIME_STEP );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( QUASI_STATIC_ANALYSIS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( NORMAL_STRESS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( TANGENTIAL_STRESS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( NORMAL_CONTACT_STRESS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( TANGENTIAL_CONTACT_STRESS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONTACT_STICK );


            KRATOS_REGISTER_IN_PYTHON_VARIABLE( WATER_PRESSURE );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( WATER_PRESSURE_DT );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( WATER_PRESSURE_NULL );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( WATER_PRESSURE_NULL_DT );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( WATER_PRESSURE_NULL_ACCELERATION );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( WATER_PRESSURE_EINS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( WATER_PRESSURE_EINS_DT );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( WATER_PRESSURE_EINS_ACCELERATION );

            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE_DT );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE_ACCELERATION );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE_NULL );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE_NULL_DT );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE_NULL_ACCELERATION );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE_EINS );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE_EINS_DT );
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( AIR_PRESSURE_EINS_ACCELERATION );

            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_OLD );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_DT );
            //  KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION);
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_NULL );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_NULL_DT );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( ACCELERATION_NULL );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_EINS );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( DISPLACEMENT_EINS_DT );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( ACCELERATION_EINS );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( FORCE_CM );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( MOMENTUM_CM );

            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ACTIVATION_LEVEL );

            KRATOS_REGISTER_IN_PYTHON_VARIABLE( BASE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( HEIGHT )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( FC )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( FT )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONCRETE_YOUNG_MODULUS_C )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONCRETE_YOUNG_MODULUS_T )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( FRACTURE_ENERGY )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( CRUSHING_ENERGY )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ELASTIC_ENERGY )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( PLASTIC_ENERGY )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( KINETIC_ENERGY )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( POTENCIAL_ENERGY )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DEFORMATION_ENERGY )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( VON_MISES_STRESS )
            // KRATOS_REGISTER_IN_PYTHON_VARIABLE   ( YIELD_STRESS)
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( PLASTIC_MODULUS )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( PLASTICITY_INDICATOR )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( LAMNDA )   // Load factor
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DAMAGE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ORTHOTROPIC_ANGLE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( VOLUMEN_FRACTION )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( MAX_INTERNAL_FRICTION_ANGLE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DILATANCY_ANGLE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( IS_DISCRETE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( MAX_DILATANCY_ANGLE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( COHESION )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ISOTROPIC_ELASTIC_LIMIT )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ORTHOTROPIC_ELASTIC_LIMIT )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( VECTOR_DAMAGE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ORTHOTROPIC_YOUNG_MODULUS_2D )   // [E1 E2 G12]
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ORTHOTROPIC_POISSON_RATIO_2D )   // [v12 v21]
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( GREEN_LAGRANGE_PLASTIC_STRAIN_TENSOR )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( PLASTIC_STRAIN_VECTOR )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( EQUIVALENT_VOLUMETRIC_PLASTIC_STRAIN )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( EQUIVALENT_DEVIATORIC_PLASTIC_STRAIN )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( PRESTRESS )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( PRESTRESS_FACTOR )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DISIPATION )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( ISOTROPIC_HARDENING_MODULUS )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( KINEMATIC_HARDENING_MODULUS )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( NODAL_STRESS )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( NODAL_STRAIN )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( NODAL_VALUES )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( NODAL_DAMAGE )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( NODAL_VOLUME )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( DAMPING_RATIO )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( TENSILE_STRENGTH )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( SHEAR_STRENGTH )
            KRATOS_REGISTER_IN_PYTHON_VARIABLE( VISCOUS_DAMPING )
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( JOINT_FORCE_REACTION );
            KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( JOINT_MOMENT_REACTION );
        }


    }  // namespace Python.

}  // namespace Kratos.

#endif // KRATOS_PYTHON defined
