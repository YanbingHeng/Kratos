//
//   Project Name:        KratosConstitutiveModelsApplication $
//   Created by:          $Author:                JMCarbonell $
//   Last modified by:    $Co-Author:                         $
//   Date:                $Date:                   April 2017 $
//   Revision:            $Revision:                      0.0 $
//
//

// System includes

#if defined(KRATOS_PYTHON)
// External includes
#include <boost/python.hpp>


// Project includes
#include "includes/define.h"
#include "constitutive_models_application.h"
#include "constitutive_models_application_variables.h"
#include "custom_python/add_custom_constitutive_laws_to_python.h"
#include "custom_python/add_custom_utilities_to_python.h"
#include "custom_python/add_custom_processes_to_python.h"

namespace Kratos
{

namespace Python
{

  using namespace boost::python;



  BOOST_PYTHON_MODULE(KratosConstitutiveModelsApplication)
  {

    class_<KratosConstitutiveModelsApplication,
	   KratosConstitutiveModelsApplication::Pointer,
	   bases<KratosApplication>, boost::noncopyable >("KratosConstitutiveModelsApplication")
      ;

    AddCustomConstitutiveLawsToPython(); 
    AddCustomUtilitiesToPython();
    AddCustomProcessesToPython();

    //registering variables in python
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( CONSTITUTIVE_LAW_NAME )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( YOUNG_MODULUS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( POISSON_RATIO )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( LAME_MU )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( LAME_LAMBDA )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( C10 )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( C20 )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( C30 )     
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( NORM_ISOCHORIC_STRESS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PLASTIC_STRAIN )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DELTA_PLASTIC_STRAIN )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( ISOTROPIC_HARDENING_MODULUS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( KINEMATIC_HARDENING_MODULUS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( HARDENING_EXPONENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( REFERENCE_HARDENING_MODULUS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( INFINITY_HARDENING_MODULUS )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DAMAGE_VARIABLE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DAMAGE_THRESHOLD )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( STRENGTH_RATIO )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( FRACTURE_ENERGY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( RESIDUAL_STRENGTH )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SOFTENING_SLOPE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( REFERENCE_TEMPERATURE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( PLASTIC_DISSIPATION )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( DELTA_PLASTIC_DISSIPATION )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( YOUNG_MODULUS_X )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( YOUNG_MODULUS_Y )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( YOUNG_MODULUS_Z )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SHEAR_MODULUS_XY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SHEAR_MODULUS_YZ )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( SHEAR_MODULUS_XZ )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( POISSON_RATIO_XY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( POISSON_RATIO_YZ )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( POISSON_RATIO_XZ )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( MATERIAL_ORIENTATION_DX )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( MATERIAL_ORIENTATION_DY )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( MATERIAL_ORIENTATION_DZ )
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE( THERMAL_EXPANSION_COEFFICIENT )
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE( SHEAR_MODULUS )
    //KRATOS_REGISTER_IN_PYTHON_VARIABLE( BULK_MODULUS )

    }


}  // namespace Python.

}  // namespace Kratos.

#endif // KRATOS_PYTHON defined
