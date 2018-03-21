//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Philipp Bucher (based on the work of Massimo Petracca and Peter Wilson)
//

// System includes

// External includes


// Project includes
#include "custom_elements/base_shell_element.h"
#include "includes/checks.h"


namespace Kratos
{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/**
 * Constructor using Geometry
 */
BaseShellElement::BaseShellElement(IndexType NewId,
    GeometryType::Pointer pGeometry,
    bool IsThickShell)
    : Element(NewId, pGeometry), mIsThickShell(IsThickShell)
{
    SetBaseMembers();
}

/**
 * Constructor using Properties
 */
BaseShellElement::BaseShellElement(IndexType NewId,
    GeometryType::Pointer pGeometry,
    PropertiesType::Pointer pProperties,
    bool IsThickShell)
    : Element(NewId, pGeometry, pProperties), mIsThickShell(IsThickShell)
{
    SetBaseMembers();
}

/**
 * Destructor
 */
BaseShellElement::~BaseShellElement() {
}

///@}
///@name Operators
///@{


///@}
///@name Operations
///@{

/**
 * this determines the elemental equation ID vector for all elemental
 * DOFs
 * @param rResult: the elemental equation ID vector
 * @param rCurrentProcessInfo: the current process info instance
 */
void BaseShellElement::EquationIdVector(EquationIdVectorType& rResult,
                                        ProcessInfo& rCurrentProcessInfo)
{
    if (rResult.size() != mNumDofs)
        rResult.resize(mNumDofs, false);

    GeometryType& geom = GetGeometry();

    for (SizeType i = 0; i < geom.size(); ++i)
    {
        const SizeType index = i * 6;
        NodeType& i_node = geom[i];

        rResult[index]     = i_node.GetDof(DISPLACEMENT_X).EquationId();
        rResult[index + 1] = i_node.GetDof(DISPLACEMENT_Y).EquationId();
        rResult[index + 2] = i_node.GetDof(DISPLACEMENT_Z).EquationId();

        rResult[index + 3] = i_node.GetDof(ROTATION_X).EquationId();
        rResult[index + 4] = i_node.GetDof(ROTATION_Y).EquationId();
        rResult[index + 5] = i_node.GetDof(ROTATION_Z).EquationId();
    }
}

/**
 * determines the elemental list of DOFs
 * @param rElementalDofList: the list of DOFs
 * @param rCurrentProcessInfo: the current process info instance
 */
void BaseShellElement::GetDofList(DofsVectorType& rElementalDofList,
                                  ProcessInfo& rCurrentProcessInfo)
{
    rElementalDofList.resize(0);
    rElementalDofList.reserve(mNumDofs);

    GeometryType& geom = GetGeometry();

    for (SizeType i = 0; i < geom.size(); ++i)
    {
        NodeType& i_node = geom[i];

        rElementalDofList.push_back(i_node.pGetDof(DISPLACEMENT_X));
        rElementalDofList.push_back(i_node.pGetDof(DISPLACEMENT_Y));
        rElementalDofList.push_back(i_node.pGetDof(DISPLACEMENT_Z));

        rElementalDofList.push_back(i_node.pGetDof(ROTATION_X));
        rElementalDofList.push_back(i_node.pGetDof(ROTATION_Y));
        rElementalDofList.push_back(i_node.pGetDof(ROTATION_Z));
    }
}

void BaseShellElement::GetValuesVector(Vector& rValues, int Step)
{
    if (rValues.size() != mNumDofs)
        rValues.resize(mNumDofs, false);

    const GeometryType& geom = GetGeometry();

    for (SizeType i = 0; i < geom.size(); ++i)
    {
        const NodeType& i_node = geom[i];
        const array_1d<double, 3>& disp = i_node.FastGetSolutionStepValue(DISPLACEMENT, Step);
        const array_1d<double, 3>& rot = i_node.FastGetSolutionStepValue(ROTATION, Step);

        const SizeType index = i * 6;
        rValues[index]     = disp[0];
        rValues[index + 1] = disp[1];
        rValues[index + 2] = disp[2];

        rValues[index + 3] = rot[0];
        rValues[index + 4] = rot[1];
        rValues[index + 5] = rot[2];
    }
}

void BaseShellElement::GetFirstDerivativesVector(Vector& rValues, int Step)
{
    if (rValues.size() != mNumDofs)
        rValues.resize(mNumDofs, false);

    const GeometryType& geom = GetGeometry();

    for (SizeType i = 0; i < geom.size(); ++i)
    {
        const NodeType& i_node = geom[i];
        const array_1d<double, 3>& vel = i_node.FastGetSolutionStepValue(VELOCITY, Step);
        // TODO also include the angular velocity

        const SizeType index = i * 6;
        rValues[index]     = vel[0];
        rValues[index + 1] = vel[1];
        rValues[index + 2] = vel[2];

        rValues[index + 3] = 0.0;
        rValues[index + 4] = 0.0;
        rValues[index + 5] = 0.0;
    }
}

void BaseShellElement::GetSecondDerivativesVector(Vector& rValues, int Step)
{
    if (rValues.size() != mNumDofs)
        rValues.resize(mNumDofs, false);

    const GeometryType & geom = GetGeometry();

    for (SizeType i = 0; i < geom.size(); ++i)
    {
        const NodeType& i_node = geom[i];
        const array_1d<double, 3>& acc = i_node.FastGetSolutionStepValue(ACCELERATION, Step);
        // TODO also include the angular acceleration

        const SizeType index = i * 6;
        rValues[index]     = acc[0];
        rValues[index + 1] = acc[1];
        rValues[index + 2] = acc[2];

        rValues[index + 3] = 0.0;
        rValues[index + 4] = 0.0;
        rValues[index + 5] = 0.0;
    }
}

void BaseShellElement::ResetConstitutiveLaw()
{
    KRATOS_TRY

    const GeometryType& geom = GetGeometry();
    const Matrix& shapeFunctionsValues = geom.ShapeFunctionsValues(GetIntegrationMethod());

    const Properties& props = GetProperties();
    for(SizeType i = 0; i < mSections.size(); i++)
        mSections[i]->ResetCrossSection(props, geom, row(shapeFunctionsValues, i));

    KRATOS_CATCH("")
}

void BaseShellElement::BaseInitializeNonLinearIteration(ProcessInfo& rCurrentProcessInfo)
{
    const GeometryType& geom = this->GetGeometry();
    const Matrix& shapeFunctionsValues = geom.ShapeFunctionsValues(GetIntegrationMethod());
    for (SizeType i = 0; i < mSections.size(); ++i)
        mSections[i]->InitializeNonLinearIteration(GetProperties(), geom, row(shapeFunctionsValues, i), rCurrentProcessInfo);
}

void BaseShellElement::BaseFinalizeNonLinearIteration(ProcessInfo& rCurrentProcessInfo)
{
    const GeometryType& geom = this->GetGeometry();
    const Matrix& shapeFunctionsValues = geom.ShapeFunctionsValues(GetIntegrationMethod());
    for (SizeType i = 0; i < mSections.size(); ++i)
        mSections[i]->FinalizeNonLinearIteration(GetProperties(), geom, row(shapeFunctionsValues, i), rCurrentProcessInfo);
}

void BaseShellElement::BaseInitializeSolutionStep(ProcessInfo& rCurrentProcessInfo)
{
	const PropertiesType& props = GetProperties();
	const GeometryType & geom = GetGeometry();
	const Matrix& shapeFunctionsValues = geom.ShapeFunctionsValues(GetIntegrationMethod());

	for (SizeType i = 0; i < mSections.size(); ++i)
		mSections[i]->InitializeSolutionStep(props, geom, row(shapeFunctionsValues, i), rCurrentProcessInfo);
}

void BaseShellElement::BaseFinalizeSolutionStep(ProcessInfo& rCurrentProcessInfo)
{
    const PropertiesType& props = GetProperties();
    const GeometryType& geom = GetGeometry();
    const Matrix& shapeFunctionsValues = geom.ShapeFunctionsValues(GetIntegrationMethod());

    for (SizeType i = 0; i < mSections.size(); i++)
        mSections[i]->FinalizeSolutionStep(props, geom, row(shapeFunctionsValues, i), rCurrentProcessInfo);
}

void BaseShellElement::CalculateLocalSystem(MatrixType& rLeftHandSideMatrix,
	VectorType& rRightHandSideVector,
	ProcessInfo& rCurrentProcessInfo)
{
    // Calculation flags
    const bool calculate_stiffness_matrix_flag = true;
    const bool calculate_residual_vector_flag = true;

	CalculateAll(rLeftHandSideMatrix, rRightHandSideVector, rCurrentProcessInfo,
                 calculate_stiffness_matrix_flag, calculate_residual_vector_flag);
}

void BaseShellElement::CalculateRightHandSide(VectorType& rRightHandSideVector,
	ProcessInfo& rCurrentProcessInfo)
{
    // Calculation flags
    const bool calculate_stiffness_matrix_flag = true; // TODO check is this can be false => see solids
    const bool calculate_residual_vector_flag = true;

	Matrix dummy;
	CalculateAll(dummy, rRightHandSideVector, rCurrentProcessInfo,
                 calculate_stiffness_matrix_flag, calculate_residual_vector_flag);
}

void BaseShellElement::CalculateMassMatrix(MatrixType& rMassMatrix, ProcessInfo& rCurrentProcessInfo)
{
    // TODO unify implementation and move it to BaseClass
}

void BaseShellElement::CalculateDampingMatrix(
    MatrixType& rDampingMatrix,
    ProcessInfo& rCurrentProcessInfo
    )
{
    KRATOS_TRY;

    if ( rDampingMatrix.size1() != mNumDofs )
        rDampingMatrix.resize( mNumDofs, mNumDofs, false );

    noalias( rDampingMatrix ) = ZeroMatrix( mNumDofs, mNumDofs );

    // 1.-Calculate StiffnessMatrix:

    MatrixType StiffnessMatrix  = Matrix();
    VectorType ResidualVector  = Vector();

    CalculateAll(StiffnessMatrix, ResidualVector, rCurrentProcessInfo, true, false);

    // 2.-Calculate MassMatrix:

    MatrixType MassMatrix  = Matrix();

    CalculateMassMatrix(MassMatrix, rCurrentProcessInfo);

    // 3.-Get Damping Coeffitients (RAYLEIGH_ALPHA, RAYLEIGH_BETA)
    double alpha = 0.0;
    if( GetProperties().Has(RAYLEIGH_ALPHA) )
        alpha = GetProperties()[RAYLEIGH_ALPHA];
    else if( rCurrentProcessInfo.Has(RAYLEIGH_ALPHA) )
        alpha = rCurrentProcessInfo[RAYLEIGH_ALPHA];

    double beta  = 0.0;
    if( GetProperties().Has(RAYLEIGH_BETA) )
        beta = GetProperties()[RAYLEIGH_BETA];
    else if( rCurrentProcessInfo.Has(RAYLEIGH_BETA) )
        beta = rCurrentProcessInfo[RAYLEIGH_BETA];

    // 4.-Compose the Damping Matrix:

    // Rayleigh Damping Matrix: alpha*M + beta*K
    noalias( rDampingMatrix ) += alpha * MassMatrix;
    noalias( rDampingMatrix ) += beta  * StiffnessMatrix;

    KRATOS_CATCH( "" )
}

int BaseShellElement::Check(const ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY;

    Element::Check(rCurrentProcessInfo);

    CheckVariables();
    CheckDofs();
    CheckProperties(rCurrentProcessInfo);

    KRATOS_ERROR_IF(GetGeometry().Area() < std::numeric_limits<double>::epsilon()*1000)
        << "Element #" << Id() << " has an Area of zero!" << std::endl;

    // TODO check ConstLaws

    return 0;

    KRATOS_CATCH( "" )
}

///@}
///@name Access
///@{


///@}
///@name Inquiry
///@{


///@}
///@name Input and output
///@{

/// Turn back information as a string.

std::string BaseShellElement::Info() const {
  std::stringstream buffer;
  buffer << "BaseShellElement #" << Id();
  return buffer.str();
}

/// Print information about this object.

void BaseShellElement::PrintInfo(std::ostream& rOStream) const {
  rOStream << "BaseShellElement #" << Id();
}

/// Print object's data.

void BaseShellElement::PrintData(std::ostream& rOStream) const {
  pGetGeometry()->PrintData(rOStream);
}

///@}
///@name Friends
///@{

///@}

///@name Protected static Member Variables
///@{

///@}
///@name Protected member Variables
///@{

///@}
///@name Protected Operators
///@{

///@}
///@name Protected Operations
///@{

void BaseShellElement::SetBaseMembers()
{
    mNumDofs = 6 * GetGeometry().PointsNumber(); // 6 dofs per node

    const GeometryType::IntegrationPointsArrayType& integrationPoints =
    GetGeometry().IntegrationPoints(mIntegrationMethod);

    mNumGPs = integrationPoints.size();
}

void BaseShellElement::CalculateAll(
    MatrixType& rLeftHandSideMatrix,
    VectorType& rRightHandSideVector,
    ProcessInfo& rCurrentProcessInfo,
    const bool CalculateStiffnessMatrixFlag,
    const bool CalculateResidualVectorFlag
    )
{
    KRATOS_ERROR << "You have called to the CalculateAll from the base class for shell elements" << std::endl;
}

void BaseShellElement::CheckVariables()
{
    KRATOS_CHECK_VARIABLE_KEY(DISPLACEMENT);
    KRATOS_CHECK_VARIABLE_KEY(ROTATION);
    KRATOS_CHECK_VARIABLE_KEY(VELOCITY);
    KRATOS_CHECK_VARIABLE_KEY(ANGULAR_VELOCITY);
    KRATOS_CHECK_VARIABLE_KEY(ACCELERATION);
    KRATOS_CHECK_VARIABLE_KEY(ANGULAR_ACCELERATION);
    KRATOS_CHECK_VARIABLE_KEY(VOLUME_ACCELERATION)
    KRATOS_CHECK_VARIABLE_KEY(DENSITY);
    KRATOS_CHECK_VARIABLE_KEY(THICKNESS);
    KRATOS_CHECK_VARIABLE_KEY(SHELL_ORTHOTROPIC_LAYERS);
    KRATOS_CHECK_VARIABLE_KEY(CONSTITUTIVE_LAW);
    KRATOS_CHECK_VARIABLE_KEY(SHELL_CROSS_SECTION);
}

void BaseShellElement::CheckDofs()
{
    GeometryType& r_geom = GetGeometry();
    // verify that the dofs exist
    for (unsigned int i = 0; i < r_geom.size(); i++)
    {
        auto& r_node = r_geom[i];
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(DISPLACEMENT, r_node);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(ROTATION, r_node);

        KRATOS_CHECK_DOF_IN_NODE(ROTATION_X, r_node);
        KRATOS_CHECK_DOF_IN_NODE(ROTATION_Y, r_node);
        KRATOS_CHECK_DOF_IN_NODE(ROTATION_Z, r_node);

        KRATOS_CHECK_DOF_IN_NODE(DISPLACEMENT_X, r_node);
        KRATOS_CHECK_DOF_IN_NODE(DISPLACEMENT_Y, r_node);
        KRATOS_CHECK_DOF_IN_NODE(DISPLACEMENT_Z, r_node);

        KRATOS_ERROR_IF(r_node.GetBufferSize() < 2) << "This Element needs "
            << "at least a buffer size = 2" << std::endl;
    }
}

void BaseShellElement::CheckProperties(const ProcessInfo& rCurrentProcessInfo)
{
    // check properties
    if(pGetProperties() == nullptr)
        KRATOS_ERROR << "Properties not provided for element " << Id() << std::endl;

    const PropertiesType & props = GetProperties();

    const GeometryType& geom = GetGeometry(); // TODO check if this can be const

    if(props.Has(SHELL_CROSS_SECTION)) // if the user specified a cross section ...
    {
        const ShellCrossSection::Pointer & section = props[SHELL_CROSS_SECTION];
        if(section == nullptr)
            KRATOS_ERROR << "SHELL_CROSS_SECTION not provided for element " << Id() << std::endl;

        section->Check(props, geom, rCurrentProcessInfo);
    }
    else if (props.Has(SHELL_ORTHOTROPIC_LAYERS))
    {
        CheckSpecificProperties();

        // perform detailed orthotropic check later in shell_cross_section
    }
    else // ... allow the automatic creation of a homogeneous section from a material and a thickness
    {
        CheckSpecificProperties();

        ShellCrossSection::Pointer dummySection = ShellCrossSection::Pointer(new ShellCrossSection());
        dummySection->BeginStack();
        dummySection->AddPly(props[THICKNESS], 0.0, 5, pGetProperties());
        dummySection->EndStack();
        dummySection->SetSectionBehavior(ShellCrossSection::Thick);
        dummySection->Check(props, geom, rCurrentProcessInfo);
    }

}

void BaseShellElement::CheckSpecificProperties()
{
    const PropertiesType & r_props = GetProperties();

    if (!r_props.Has(CONSTITUTIVE_LAW))
        KRATOS_ERROR << "CONSTITUTIVE_LAW not provided for element " << Id() << std::endl;
    const ConstitutiveLaw::Pointer& claw = r_props[CONSTITUTIVE_LAW];
    if (claw == nullptr)
        KRATOS_ERROR << "CONSTITUTIVE_LAW not provided for element " << Id() << std::endl;

    if(!r_props.Has(THICKNESS))
        KRATOS_ERROR << "THICKNESS not provided for element " << Id() << std::endl;
    if(r_props[THICKNESS] <= 0.0)
        KRATOS_ERROR << "wrong THICKNESS value provided for element " << Id() << std::endl;

    if(!r_props.Has(DENSITY))
        KRATOS_ERROR << "DENSITY not provided for element " << Id() << std::endl;
    if(r_props[DENSITY] < 0.0)
        KRATOS_ERROR << "wrong DENSITY value provided for element " << Id() << std::endl;

    if(mIsThickShell)
    {
        // Check constitutive law has been verified with Stenberg stabilization
        // applicable for 5-parameter shells only.
        bool stenberg_stabilization_suitable = false;
        claw->GetValue(STENBERG_SHEAR_STABILIZATION_SUITABLE, stenberg_stabilization_suitable);
        if (!stenberg_stabilization_suitable)
        {
            std::cout << "\nWARNING:\nThe current constitutive law has not been checked with Stenberg shear stabilization."
                << "\nPlease check results carefully."
                << std::endl;
        }
    }
}

///@}
///@name Protected  Access
///@{

///@}
///@name Protected Inquiry
///@{

///@}
///@name Protected LifeCycle
///@{

///@}

///@name Static Member Variables
///@{

///@}
///@name Member Variables
///@{

///@}
///@name Private Operators
///@{

///@}
///@name Private Operations
///@{

///@}
///@name Serialization
///@{

void BaseShellElement::save(Serializer& rSerializer) const {
  KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Element);
  rSerializer.save("Sections", mSections);
  rSerializer.save("NumDofs", mNumDofs);
  rSerializer.save("NumGPs", mNumGPs);
  rSerializer.save("IsThickShell", mIsThickShell);
  rSerializer.save("IntM", (int)mIntegrationMethod);
}

void BaseShellElement::load(Serializer& rSerializer) {
  KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Element);
  rSerializer.load("Sections", mSections);
  rSerializer.load("NumDofs", mNumDofs);
  rSerializer.load("NumGPs", mNumGPs);
  rSerializer.load("IsThickShell", mIsThickShell);
  int temp;
  rSerializer.load("IntM", temp);
  mIntegrationMethod = (IntegrationMethod)temp;
}

///@}
///@name Private  Access
///@{

///@}
///@name Private Inquiry
///@{

///@}
///@name Un accessible methods
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name Input and output
///@{

/// input stream function
inline std::istream & operator >> (std::istream& rIStream, BaseShellElement& rThis);

/// output stream function
inline std::ostream & operator << (std::ostream& rOStream, const BaseShellElement& rThis) {
  rThis.PrintInfo(rOStream);
  rOStream << " : " << std::endl;
  rThis.PrintData(rOStream);
  return rOStream;
}

} // namespace Kratos.
