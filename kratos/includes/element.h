//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ \.
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand
//


#if !defined(KRATOS_ELEMENT_H_INCLUDED )
#define  KRATOS_ELEMENT_H_INCLUDED

// System includes
#include <string>
#include <iostream>
#include <sstream>
#include <cstddef>

// External includes

// Project includes
#include "includes/define.h"
#include "includes/node.h"
#include "geometries/geometry.h"
#include "includes/properties.h"
#include "includes/process_info.h"
#include "includes/geometrical_object.h"
#include "utilities/indexed_object.h"
#include "containers/flags.h"
#include "containers/weak_pointer_vector.h"
#include "includes/constitutive_law.h"

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

/// Base class for all Elements.

/**
 * This is the base class for all elements used in KRATOS
 * Elements inherited from this class have to reimplement
 * all public functions that are needed to perform their designated
 * tasks. Due to a dummy implementation of every function though,
 * not all of them have to be implemented if they are not needed for
 * the actual problem
 */
class Element : public GeometricalObject, public Flags
{
public:
    ///@name Type Definitions
    ///@{
    /// Pointer definition of Element
    KRATOS_CLASS_POINTER_DEFINITION(Element);

    ///definition of element type
    typedef Element ElementType;

    ///base type: an GeometricalObject that automatically has a unique number
    typedef GeometricalObject BaseType;

    ///definition of node type (default is: Node<3>)
    typedef Node < 3 > NodeType;

    /**
     * Properties are used to store any parameters
     * related to the constitutive law
     */
    typedef Properties PropertiesType;

    ///definition of the geometry type with given NodeType
    typedef Geometry<NodeType> GeometryType;

    ///definition of nodes container type, redefined from GeometryType
    typedef Geometry<NodeType>::PointsArrayType NodesArrayType;

    typedef Vector VectorType;

    typedef Matrix MatrixType;

    typedef std::size_t IndexType;

    typedef std::size_t SizeType;

    typedef std::vector<std::size_t> EquationIdVectorType;

    typedef std::vector< Dof<double>::Pointer > DofsVectorType;

    typedef PointerVectorSet<Dof<double>, IndexedObject> DofsArrayType;

    typedef VectorMap<IndexType, DataValueContainer> SolutionStepsElementalDataContainerType;

    ///Type definition for integration methods
    typedef GeometryData::IntegrationMethod IntegrationMethod;

    typedef GeometryData GeometryDataType;
    ///@}

    ///@name Life Cycle
    ///@{

    /**
     * ELEMENTS inherited from this class have to implement next
     * contructors, copy constructors and destructor: MANDATORY
     */

    /**
     * Constructor.
     */
    Element(IndexType NewId = 0)
        : BaseType(NewId)
        , Flags()
        , mpProperties(nullptr)
    {
    }

    /**
     * Constructor using an array of nodes
     */
    Element(IndexType NewId, const NodesArrayType& ThisNodes)
        : BaseType(NewId,GeometryType::Pointer(new GeometryType(ThisNodes)))
        , Flags()
        , mpProperties(nullptr)
    {
    }

    /**
     * Constructor using Geometry
     */
    Element(IndexType NewId, GeometryType::Pointer pGeometry)
        : BaseType(NewId,pGeometry)
        , Flags()
        , mpProperties(nullptr)
    {
    }

    /**
     * Constructor using Properties
     */
    Element(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties)
        : BaseType(NewId,pGeometry)
        , Flags()
        , mpProperties(pProperties)
    {
    }

    /// Copy constructor.

    Element(Element const& rOther)
        : BaseType(rOther)
        , Flags(rOther)
        , mData(rOther.mData)
        , mpProperties(rOther.mpProperties)
    {
    }

    /// Destructor.

    ~Element() override
    {
    }

    ///@}
    ///@name Operators
    ///@{

    /**
     * ELEMENTS inherited from this class have to implement next
     * assignment operator: MANDATORY
     */

    /// Assignment operator.

    Element & operator=(Element const& rOther)
    {
        BaseType::operator=(rOther);
        Flags::operator =(rOther);
        mpProperties = rOther.mpProperties;
        return *this;
    }

    ///@}
    ///@name Informations
    ///@{

    /** Dimensional space of the element geometry
	@return SizeType, working space dimension of this geometry.
    */

    SizeType WorkingSpaceDimension() const
    {
         return pGetGeometry()->WorkingSpaceDimension();
    }

    ///@}
    ///@name Operations
    ///@{

    /**
     * ELEMENTS inherited from this class have to implement next
     * Create and Clone methods: MANDATORY
     */

    /**
     * creates a new element pointer
     * @param NewId the ID of the new element
     * @param ThisNodes the nodes of the new element
     * @param pProperties the properties assigned to the new element
     * @return a Pointer to the new element
     */
    virtual Pointer Create(IndexType NewId, NodesArrayType const& ThisNodes,
                           PropertiesType::Pointer pProperties) const
    {
        KRATOS_TRY
        return Kratos::make_shared<Element>(NewId, GetGeometry().Create(ThisNodes), pProperties);
        KRATOS_CATCH("");
    }

    /**
     * creates a new element pointer
     * @param NewId the ID of the new element
     * @param pGeom the geometry to be employed
     * @param pProperties the properties assigned to the new element
     * @return a Pointer to the new element
     */
    virtual Pointer Create(IndexType NewId,
                           GeometryType::Pointer pGeom,
                           PropertiesType::Pointer pProperties) const
    {
        KRATOS_TRY
        return Kratos::make_shared<Element>(NewId, pGeom, pProperties);
        KRATOS_CATCH("");
    }

    /**
     * creates a new element pointer and clones the previous element data
     * @param NewId the ID of the new element
     * @param ThisNodes the nodes of the new element
     * @param pProperties the properties assigned to the new element
     * @return a Pointer to the new element
     */
    virtual Pointer Clone (IndexType NewId, NodesArrayType const& ThisNodes) const
    {
        KRATOS_TRY
	std::cout<<" Call base class element Clone "<<std::endl;
        return Element::Pointer(new Element(NewId, GetGeometry().Create(ThisNodes), pGetProperties()));
        KRATOS_CATCH("");
    }


    /**
     * ELEMENTS inherited from this class have to implement next
     * EquationIdVector and GetDofList methods: MANDATORY
     */

    /**
     * this determines the elemental equation ID vector for all elemental
     * DOFs
     * @param rResult the elemental equation ID vector
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void EquationIdVector(EquationIdVectorType& rResult,
                                  ProcessInfo& rCurrentProcessInfo)
    {
        if (rResult.size() != 0)
            rResult.resize(0);
    }

    /**
     * determines the elemental list of DOFs
     * @param ElementalDofList the list of DOFs
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void GetDofList(DofsVectorType& rElementalDofList,
                            ProcessInfo& rCurrentProcessInfo)
    {
        if (rElementalDofList.size() != 0)
            rElementalDofList.resize(0);
    }

    /**
     * returns the used integration method. In the general case this is the
     * default integration method of the used geometry. I an other integration
     * method is used the method has to be overwritten within the element
     * @return default integration method of the used Geometry
     */
    virtual IntegrationMethod GetIntegrationMethod() const
    {
        return pGetGeometry()->GetDefaultIntegrationMethod();
    }

    /**
     * ELEMENTS inherited from this class must implement this methods
     * if they need the values of the time derivatives of any of the dof
     * set by the element. If the derivatives do not exist can set to zero
     * these methods are: MANDATORY ( when compatibility with dynamics is required )
     */

    /**
     * Getting method to obtain the variable which defines the degrees of freedom
     */
    virtual void GetValuesVector(Vector& values, int Step = 0)
    {
    }

    /**
     * Getting method to obtain the time derivative of variable which defines the degrees of freedom
     */
    virtual void GetFirstDerivativesVector(Vector& values, int Step = 0)
    {
    }

    /**
     * Getting method to obtain the second time derivative of variable which defines the degrees of freedom
     */
    virtual void GetSecondDerivativesVector(Vector& values, int Step = 0)
    {
    }

    /**
     * ELEMENTS inherited from this class must implement next methods
     * Initialize, ResetConstitutiveLaw, CleanMemory
     * if the element needs to perform any operation before any calculation is done
     * reset material and constitutive parameters
     * or clean memory deleting obsolete variables
     * these methods are: OPTIONAL
     */

    /**
     * is called to initialize the element
     * if the element needs to perform any operation before any calculation is done
     * the elemental variables will be initialized and set using this method
     */
    virtual void Initialize()
    {
    }


    /**
     * is called to reset the constitutive law parameters and the material properties
     * the elemental variables will be changed and reset using this method
     */
    virtual void ResetConstitutiveLaw()
    {
    }

    /**
     * deletes all obsolete data from memory
     */
    virtual void CleanMemory()
    {
    }

    /**
     * ELEMENTS inherited from this class must implement next methods
     * InitializeSolutionStep, FinalizeSolutionStep,
     * InitializeNonLinearIteration, FinalizeNonLinearIteration
     * if the element needs to perform any operation before and after the solution step
     * if the element needs to perform any operation before and after the solution iteration
     * these methods are: OPTIONAL
     */


    /**
     * this is called in the beginning of each solution step
     */
    virtual void InitializeSolutionStep(ProcessInfo& rCurrentProcessInfo)
    {
    }


    /**
     * this is called for non-linear analysis at the beginning of the iteration process
     */
    virtual void InitializeNonLinearIteration(ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called for non-linear analysis at the end of the iteration process
     */
    virtual void FinalizeNonLinearIteration(ProcessInfo& rCurrentProcessInfo)
    {
    }


    /**
     * this is called at the end of each solution step
     */
    virtual void FinalizeSolutionStep(ProcessInfo& rCurrentProcessInfo)
    {
    }


    /**
     * ELEMENTS inherited from this class have to implement next
     * CalculateLocalSystem, CalculateLeftHandSide and CalculateRightHandSide methods
     * they can be managed internally with a private method to do the same calculations
     * only once: MANDATORY
     */

    /**
     * this is called during the assembling process in order
     * to calculate all elemental contributions to the global system
     * matrix and the right hand side
     * @param rLeftHandSideMatrix the elemental left hand side matrix
     * @param rRightHandSideVector the elemental right hand side
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix,
                                      VectorType& rRightHandSideVector,
                                      ProcessInfo& rCurrentProcessInfo)
    {
       if (rLeftHandSideMatrix.size1() != 0)
	  rLeftHandSideMatrix.resize(0, 0, false);
        if (rRightHandSideVector.size() != 0)
	  rRightHandSideVector.resize(0, false);
    }

    /**
     * this function provides a more general interface to the element.
     * it is designed so that rLHSvariables and rRHSvariables are passed TO the element
     * thus telling what is the desired output
     * @param rLeftHandSideMatrices container with the output left hand side matrices
     * @param rLHSVariables paramter describing the expected LHSs
     * @param rRightHandSideVectors container for the desired RHS output
     * @param rRHSVariables parameter describing the expected RHSs
     */
    virtual void CalculateLocalSystem(std::vector< MatrixType >& rLeftHandSideMatrices,
                                      const std::vector< Variable< MatrixType > >& rLHSVariables,
                                      std::vector< VectorType >& rRightHandSideVectors,
                                      const std::vector< Variable< VectorType > >& rRHSVariables,
                                      ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called during the assembling process in order
     * to calculate the elemental left hand side matrix only
     * @param rLeftHandSideMatrix the elemental left hand side matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateLeftHandSide(MatrixType& rLeftHandSideMatrix,
                                       ProcessInfo& rCurrentProcessInfo)
    {
        if (rLeftHandSideMatrix.size1() != 0)
	  rLeftHandSideMatrix.resize(0, 0, false);
    }

    /**
     * this function provides a more general interface to the element.
     * it is designed so that rLHSvariables are passed TO the element
     * thus telling what is the desired output
     * @param rLeftHandSideMatrices container for the desired LHS output
     * @param rLHSVariables parameter describing the expected LHSs
     */
    virtual void CalculateLeftHandSide(std::vector< MatrixType >& rLeftHandSideMatrices,
					const std::vector< Variable< MatrixType > >& rLHSVariables,
					ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called during the assembling process in order
     * to calculate the elemental right hand side vector only
     * @param rRightHandSideVector the elemental right hand side vector
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateRightHandSide(VectorType& rRightHandSideVector,
                                        ProcessInfo& rCurrentProcessInfo)
    {
        if (rRightHandSideVector.size() != 0)
	  rRightHandSideVector.resize(0, false);
    }

    /**
     * this function provides a more general interface to the element.
     * it is designed so that rRHSvariables are passed TO the element
     * thus telling what is the desired output
     * @param rRightHandSideVectors container for the desired RHS output
     * @param rRHSVariables parameter describing the expected RHSs
     */
    virtual void CalculateRightHandSide(std::vector< VectorType >& rRightHandSideVectors,
					const std::vector< Variable< VectorType > >& rRHSVariables,
					ProcessInfo& rCurrentProcessInfo)
    {
    }


    /**
     * ELEMENTS inherited from this class must implement this methods
     * if they need to add dynamic element contributions
     * note: first derivatives means the velocities if the displacements are the dof of the analysis
     * note: time integration parameters must be set in the rCurrentProcessInfo before calling these methods
     * CalculateFirstDerivativesContributions,
     * CalculateFirstDerivativesLHS, CalculateFirstDerivativesRHS methods are : OPTIONAL
     */


    /**
     * this is called during the assembling process in order
     * to calculate the first derivatives contributions for the LHS and RHS
     * @param rLeftHandSideMatrix the elemental left hand side matrix
     * @param rRightHandSideVector the elemental right hand side
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateFirstDerivativesContributions(MatrixType& rLeftHandSideMatrix,
							VectorType& rRightHandSideVector,
							ProcessInfo& rCurrentProcessInfo)
    {
       if (rLeftHandSideMatrix.size1() != 0)
	  rLeftHandSideMatrix.resize(0, 0, false);
        if (rRightHandSideVector.size() != 0)
	  rRightHandSideVector.resize(0, false);
    }

    /**
     * this is called during the assembling process in order
     * to calculate the elemental left hand side matrix for the first derivatives constributions
     * @param rLeftHandSideMatrix the elemental left hand side matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateFirstDerivativesLHS(MatrixType& rLeftHandSideMatrix,
					      ProcessInfo& rCurrentProcessInfo)
    {
        if (rLeftHandSideMatrix.size1() != 0)
	  rLeftHandSideMatrix.resize(0, 0, false);
    }


    /**
     * this is called during the assembling process in order
     * to calculate the elemental right hand side vector for the first derivatives constributions
     * @param rRightHandSideVector the elemental right hand side vector
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateFirstDerivativesRHS(VectorType& rRightHandSideVector,
					      ProcessInfo& rCurrentProcessInfo)
    {
        if (rRightHandSideVector.size() != 0)
	  rRightHandSideVector.resize(0, false);
    }



    /**
     * ELEMENTS inherited from this class must implement this methods
     * if they need to add dynamic element contributions
     * note: second derivatives means the accelerations if the displacements are the dof of the analysis
     * note: time integration parameters must be set in the rCurrentProcessInfo before calling these methods
     * CalculateSecondDerivativesContributions,
     * CalculateSecondDerivativesLHS, CalculateSecondDerivativesRHS methods are : OPTIONAL
     */


   /**
     * this is called during the assembling process in order
     * to calculate the second derivative contributions for the LHS and RHS
     * @param rLeftHandSideMatrix the elemental left hand side matrix
     * @param rRightHandSideVector the elemental right hand side
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateSecondDerivativesContributions(MatrixType& rLeftHandSideMatrix,
							 VectorType& rRightHandSideVector,
							 ProcessInfo& rCurrentProcessInfo)
    {
       if (rLeftHandSideMatrix.size1() != 0)
	  rLeftHandSideMatrix.resize(0, 0, false);
        if (rRightHandSideVector.size() != 0)
	  rRightHandSideVector.resize(0, false);
    }


    /**
     * this is called during the assembling process in order
     * to calculate the elemental left hand side matrix for the second derivatives constributions
     * @param rLeftHandSideMatrix the elemental left hand side matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateSecondDerivativesLHS(MatrixType& rLeftHandSideMatrix,
					       ProcessInfo& rCurrentProcessInfo)
    {
        if (rLeftHandSideMatrix.size1() != 0)
	  rLeftHandSideMatrix.resize(0, 0, false);
    }


    /**
     * this is called during the assembling process in order
     * to calculate the elemental right hand side vector for the second derivatives constributions
     * @param rRightHandSideVector the elemental right hand side vector
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateSecondDerivativesRHS(VectorType& rRightHandSideVector,
					       ProcessInfo& rCurrentProcessInfo)
    {
        if (rRightHandSideVector.size() != 0)
	  rRightHandSideVector.resize(0, false);
    }



    /**
     * ELEMENTS inherited from this class must implement this methods
     * if they need to add dynamic element contributions
     * CalculateMassMatrix and CalculateDampingMatrix methods are: OPTIONAL
     */

    /**
     * this is called during the assembling process in order
     * to calculate the elemental mass matrix
     * @param rMassMatrix the elemental mass matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateMassMatrix(MatrixType& rMassMatrix, ProcessInfo& rCurrentProcessInfo)
    {
        if (rMassMatrix.size1() != 0)
	  rMassMatrix.resize(0, 0, false);
    }


    /**
     * this is called during the assembling process in order
     * to calculate the elemental damping matrix
     * @param rDampingMatrix the elemental damping matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateDampingMatrix(MatrixType& rDampingMatrix, ProcessInfo& rCurrentProcessInfo)
    {
        if (rDampingMatrix.size1() != 0)
	  rDampingMatrix.resize(0, 0, false);
    }



    /**
     * ELEMENTS inherited from this class must implement this methods
     * if they need to write something at the element geometry nodes
     * AddExplicitContribution methods are: OPTIONAL ( avoid to use them if is not needed )
     */

    /**
     * this is called during the assembling process in order
     * to calculate the elemental contribution in explicit calculation.
     * NodalData is modified Inside the function, so the
     * The "AddEXplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH AN ELEMENT
     * IS ALLOWED TO WRITE ON ITS NODES.
     * the caller is expected to ensure thread safety hence
     * SET/UNSETLOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
      * @param rCurrentProcessInfo the current process info instance
     */
    virtual void AddExplicitContribution(ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this function is designed to make the element to assemble an rRHS vector
     * identified by a variable rRHSVariable by assembling it to the nodes on the variable
     * rDestinationVariable.
     * The "AddEXplicit" FUNCTIONS THE ONLY FUNCTIONS IN WHICH AN ELEMENT
     * IS ALLOWED TO WRITE ON ITS NODES.
     * the caller is expected to ensure thread safety hence
     * SET/UNSETLOCK MUST BE PERFORMED IN THE STRATEGY BEFORE CALLING THIS FUNCTION
     * @param rRHSVector input variable containing the RHS vector to be assembled
     * @param rRHSVariable variable describing the type of the RHS vector to be assembled
     * @param rDestinationVariable variable in the database to which the rRHSVector will be assembled
      * @param rCurrentProcessInfo the current process info instance
     */
    virtual void AddExplicitContribution(const VectorType& rRHSVector, const Variable<VectorType>& rRHSVariable, Variable<double >& rDestinationVariable, const ProcessInfo& rCurrentProcessInfo)
    {
        KRATOS_THROW_ERROR(std::logic_error, "base element class is not able to assemble rRHS to the desired variable. destination variable is ",rDestinationVariable)
    }

    virtual void AddExplicitContribution(const VectorType& rRHSVector, const Variable<VectorType>& rRHSVariable, Variable<array_1d<double,3> >& rDestinationVariable, const ProcessInfo& rCurrentProcessInfo)
    {
         KRATOS_THROW_ERROR(std::logic_error, "base element class is not able to assemble rRHS to the desired variable. destination variable is ",rDestinationVariable)
    }

    virtual void AddExplicitContribution(const MatrixType& rLHSMatrix, const Variable<MatrixType>& rLHSVariable, Variable<Matrix>& rDestinationVariable, const ProcessInfo& rCurrentProcessInfo)
    {
         KRATOS_THROW_ERROR(std::logic_error, "base element class is not able to assemble rLHS to the desired variable. destination variable is ",rDestinationVariable)
    }

    /**
     * Calculate a Element variable usually associated to a integration point
     * the Output is given on integration points and characterizes the element
     * Calculate(..) methods are: OPTIONAL
     */

    virtual void Calculate(const Variable<double>& rVariable,
                           double& Output,
                           const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void Calculate(const Variable<array_1d<double, 3 > >& rVariable,
                           array_1d<double, 3 > & Output,
                           const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void Calculate(const Variable<Vector >& rVariable,
                           Vector& Output,
                           const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void Calculate(const Variable<Matrix >& rVariable,
                           Matrix& Output,
                           const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    /**
     * Calculate variables on Integration points.
     * This gives access to variables computed in the constitutive law on each integration point.
     * Specialisations of element must specify the actual interface to the integration points!
     * Note, that these functions expect a std::vector of values for the specified variable type that
     * contains a value for each integration point!
     * CalculateValueOnIntegrationPoints: calculates the values of given Variable.
     * these methods are: OPTIONAL
     */

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void CalculateOnIntegrationPoints(const Variable<bool>& rVariable,
					     std::vector<bool>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void CalculateOnIntegrationPoints(const Variable<double>& rVariable,
					      std::vector<double>& rOutput,
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void CalculateOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable,
					      std::vector< array_1d<double, 3 > >& rOutput,
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void CalculateOnIntegrationPoints(const Variable<array_1d<double, 6 > >& rVariable,
					      std::vector< array_1d<double, 6 > >& rOutput,
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void CalculateOnIntegrationPoints(const Variable<Vector >& rVariable,
					      std::vector< Vector >& rOutput,
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void CalculateOnIntegrationPoints(const Variable<Matrix >& rVariable,
					      std::vector< Matrix >& rOutput,
					      const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * Access for variables on Integration points.
     * This gives access to variables stored in the constitutive law on each integration point.
     * Specializations of element must specify the actual interface to the integration points!
     * Note, that these functions expect a std::vector of values for the specified variable type that
     * contains a value for each integration point!
     * SetValueOnIntegrationPoints: set the values for given Variable.
     * GetValueOnIntegrationPoints: get the values for given Variable.
     * these methods are: OPTIONAL
     */

    //SET ON INTEGRATION POINTS - METHODS

    /// This method is deprecated, use SetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void SetValueOnIntegrationPoints(const Variable<int>& rVariable,
					     std::vector<int>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use SetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void SetValueOnIntegrationPoints(const Variable<double>& rVariable,
					     std::vector<double>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use SetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void SetValueOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable,
					     std::vector<array_1d<double, 3 > > rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use SetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void SetValueOnIntegrationPoints(const Variable<array_1d<double, 6 > >& rVariable,
					     std::vector<array_1d<double, 6 > > rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use SetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void SetValueOnIntegrationPoints(const Variable<Vector>& rVariable,
					     std::vector<Vector>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use SetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void SetValueOnIntegrationPoints(const Variable<Matrix>& rVariable,
					     std::vector<Matrix>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use SetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void SetValueOnIntegrationPoints(const Variable<ConstitutiveLaw::Pointer>& rVariable,
					     std::vector<ConstitutiveLaw::Pointer>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    //GET ON INTEGRATION POINTS METHODS

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void GetValueOnIntegrationPoints(const Variable<bool>& rVariable,
					     std::vector<bool>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void GetValueOnIntegrationPoints(const Variable<int>& rVariable,
					     std::vector<int>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void GetValueOnIntegrationPoints(const Variable<double>& rVariable,
					     std::vector<double>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void GetValueOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable,
					     std::vector<array_1d<double, 3 > >& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void GetValueOnIntegrationPoints(const Variable<array_1d<double, 6 > >& rVariable,
					     std::vector<array_1d<double, 6 > >& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void GetValueOnIntegrationPoints(const Variable<Vector>& rVariable,
					     std::vector<Vector>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void GetValueOnIntegrationPoints(const Variable<Matrix>& rVariable,
					     std::vector<Matrix>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /// This method is deprecated, use GetValuesOnIntegrationPoints instead
    KRATOS_DEPRECATED virtual void GetValueOnIntegrationPoints(const Variable<ConstitutiveLaw::Pointer>& rVariable,
					     std::vector<ConstitutiveLaw::Pointer>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void SetValuesOnIntegrationPoints(const Variable<bool>& rVariable,
					     std::vector<bool>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {}

    virtual void SetValuesOnIntegrationPoints(const Variable<int>& rVariable,
					     std::vector<int>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {}

    virtual void SetValuesOnIntegrationPoints(const Variable<double>& rVariable,
					     std::vector<double>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {}

    virtual void SetValuesOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable,
					     std::vector<array_1d<double, 3 > > rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {}

    virtual void SetValuesOnIntegrationPoints(const Variable<array_1d<double, 6 > >& rVariable,
					     std::vector<array_1d<double, 6 > > rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {}

    virtual void SetValuesOnIntegrationPoints(const Variable<Vector>& rVariable,
					     std::vector<Vector>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {}

    virtual void SetValuesOnIntegrationPoints(const Variable<Matrix>& rVariable,
					     std::vector<Matrix>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {}

    virtual void SetValuesOnIntegrationPoints(const Variable<ConstitutiveLaw::Pointer>& rVariable,
					     std::vector<ConstitutiveLaw::Pointer>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {}

    //GET ON INTEGRATION POINTS METHODS

    virtual void GetValuesOnIntegrationPoints(const Variable<bool>& rVariable,
					     std::vector<bool>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValuesOnIntegrationPoints(const Variable<int>& rVariable,
					     std::vector<int>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValuesOnIntegrationPoints(const Variable<double>& rVariable,
					     std::vector<double>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValuesOnIntegrationPoints(const Variable<array_1d<double, 3 > >& rVariable,
					     std::vector<array_1d<double, 3 > >& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValuesOnIntegrationPoints(const Variable<array_1d<double, 6 > >& rVariable,
					     std::vector<array_1d<double, 6 > >& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValuesOnIntegrationPoints(const Variable<Vector>& rVariable,
					     std::vector<Vector>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValuesOnIntegrationPoints(const Variable<Matrix>& rVariable,
					     std::vector<Matrix>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    virtual void GetValuesOnIntegrationPoints(const Variable<ConstitutiveLaw::Pointer>& rVariable,
					     std::vector<ConstitutiveLaw::Pointer>& rValues,
					     const ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * This method provides the place to perform checks on the completeness of the input
     * and the compatibility with the problem options as well as the contitutive laws selected
     * It is designed to be called only once (or anyway, not often) typically at the beginning
     * of the calculations, so to verify that nothing is missing from the input
     * or that no common error is found.
     * @param rCurrentProcessInfo
     * this method is: MANDATORY
     */

    virtual int Check(const ProcessInfo& rCurrentProcessInfo)
    {
        KRATOS_TRY

        KRATOS_ERROR_IF( this->Id() < 1 ) << "Element found with Id " << this->Id() << std::endl;

        const double domain_size = this->GetGeometry().DomainSize();
        KRATOS_ERROR_IF( domain_size <= 0.0 ) << "Element " << this->Id() << " has non-positive size " << domain_size << std::endl;

        return 0;

        KRATOS_CATCH("")
    }

    //METHODS TO BE CLEANED: DEPRECATED start

    //NOTE: They will be deleted in December, 2015


    /**
     * ELEMENTS inherited from this class must implement this methods
     * if they need to add dynamic element contributions
     * MassMatrix, AddMassMatrix, DampMatrix, AddInertiaForces methods are: OPTIONAL and OBSOLETE
     */

    /**
     * this is called during the assembling process in order
     * to calculate the elemental mass matrix
     * @param rMassMatrix the elemental mass matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void MassMatrix(MatrixType& rMassMatrix, ProcessInfo& rCurrentProcessInfo)
    {
        if (rMassMatrix.size1() != 0)
	  rMassMatrix.resize(0, 0, false);
    }

    /**
     * adds the mass matrix scaled by a given factor to the LHS
     * @param rLeftHandSideMatrix the elemental LHS matrix
     * @param coeff the given factor
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void AddMassMatrix(MatrixType& rLeftHandSideMatrix,
                               double coeff, ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * this is called during the assembling process in order
     * to calculate the elemental damping matrix
     * @param rDampMatrix the elemental damping matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void DampMatrix(MatrixType& rDampMatrix, ProcessInfo& rCurrentProcessInfo)
    {
        if (rDampMatrix.size1() != 0)
	  rDampMatrix.resize(0, 0, false);
    }

    /**
     * adds the inertia forces to the RHS --> performs residua = static_residua - coeff*M*acc
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void AddInertiaForces(VectorType& rRightHandSideVector, double coeff,
                                  ProcessInfo& rCurrentProcessInfo)
    {
    }

    /**
     * Calculate Damp matrix and add velocity contribution to RHS
     * @param rDampingMatrix the velocity-proportional "damping" matrix
     * @param rRightHandSideVector the elemental right hand side matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    virtual void CalculateLocalVelocityContribution(MatrixType& rDampingMatrix,
            VectorType& rRightHandSideVector, ProcessInfo& rCurrentProcessInfo)
    {
        if (rDampingMatrix.size1() != 0)
	  rDampingMatrix.resize(0, 0, false);

    }

    /**
     * Calculate the transposed gradient of the element's residual w.r.t. design variable.
     */
    virtual void CalculateSensitivityMatrix(const Variable<double>& rDesignVariable,
                                            Matrix& rOutput,
                                            const ProcessInfo& rCurrentProcessInfo)
    {
        if (rOutput.size1() != 0)
            rOutput.resize(0, 0, false);
    }

    /**
     * Calculate the transposed gradient of the element's residual w.r.t. design variable.
     */
    virtual void CalculateSensitivityMatrix(const Variable<array_1d<double,3> >& rDesignVariable,
                                            Matrix& rOutput,
                                            const ProcessInfo& rCurrentProcessInfo)
    {
        if (rOutput.size1() != 0)
            rOutput.resize(0, 0, false);
    }


    //METHODS TO BE CLEANED: DEPRECATED end

    ///@}
    ///@name Access
    ///@{

    PropertiesType::Pointer pGetProperties()
    {
        KRATOS_DEBUG_ERROR_IF(mpProperties == nullptr)
            << "Tryining to get the properties of " << Info()
            << ", which are uninitialized." << std::endl;
        return mpProperties;
    }

    const PropertiesType::Pointer pGetProperties() const
    {
        KRATOS_DEBUG_ERROR_IF(mpProperties == nullptr)
            << "Tryining to get the properties of " << Info()
            << ", which are uninitialized." << std::endl;
        return mpProperties;
    }

    PropertiesType& GetProperties()
    {
        KRATOS_DEBUG_ERROR_IF(mpProperties == nullptr)
            << "Tryining to get the properties of " << Info()
            << ", which are uninitialized." << std::endl;
        return *mpProperties;
    }

    PropertiesType const& GetProperties() const
    {
        KRATOS_DEBUG_ERROR_IF(mpProperties == nullptr)
            << "Tryining to get the properties of " << Info()
            << ", which are uninitialized." << std::endl;
        return *mpProperties;
    }

    void SetProperties(PropertiesType::Pointer pProperties)
    {
        mpProperties = pProperties;
    }

    ///@}
    ///@name Elemental Data
    ///@{

    /**
     * Access Data:
     */
    DataValueContainer& Data()
    {
        return mData;
    }

    DataValueContainer const& GetData() const
    {
      return mData;
    }

    void SetData(DataValueContainer const& rThisData)
    {
      mData = rThisData;
    }

    /**
     * Check if the Data exists with Has(..) methods:
     */
    template<class TDataType> bool Has(const Variable<TDataType>& rThisVariable) const
    {
        return mData.Has(rThisVariable);
    }

    template<class TAdaptorType> bool Has(
        const VariableComponent<TAdaptorType>& rThisVariable) const
    {
        return mData.Has(rThisVariable);
    }

    /**
     * Set Data with SetValue and the Variable to set:
     */
    template<class TVariableType> void SetValue(
        const TVariableType& rThisVariable,
        typename TVariableType::Type const& rValue)
    {
        mData.SetValue(rThisVariable, rValue);
    }

    /**
     * Get Data with GetValue and the Variable to get:
     */
    template<class TVariableType> typename TVariableType::Type& GetValue(
        const TVariableType& rThisVariable)
    {
        return mData.GetValue(rThisVariable);
    }

    template<class TVariableType> typename TVariableType::Type const& GetValue(
        const TVariableType& rThisVariable) const
    {
        return mData.GetValue(rThisVariable);
    }

    ///@}
    ///@name Flags
    ///@{

    Flags& GetFlags()
      {
	return *this;
      }

    Flags const& GetFlags() const
    {
      return *this;
    }

    void SetFlags(Flags const& rThisFlags)
    {
      Flags::operator=(rThisFlags);
    }

    ///@}
    ///@name Inquiry
    ///@{

    /// Check that the Element has a correctly initialized pointer to a Properties instance.
    bool HasProperties() const
    {
        return mpProperties != nullptr;
    }

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.

    std::string Info() const override
    {
        std::stringstream buffer;
        buffer << "Element #" << Id();
        return buffer.str();
    }

    /// Print information about this object.

    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "Element #" << Id();
    }

    /// Print object's data.

    void PrintData(std::ostream& rOStream) const override
    {
        pGetGeometry()->PrintData(rOStream);
    }

    ///@}
    ///@name Friends
    ///@{
    ///@}

protected:
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

private:
    ///@name Static Member Variables
    ///@{
    ///@}
    ///@name Member Variables
    ///@{

    /**
     * pointer to the data related to this element
     */
    DataValueContainer mData;

    /**
     * pointer to the element's properties
     */
    Properties::Pointer mpProperties;

    ///@}
    ///@name Private Operators
    ///@{
    ///@}
    ///@name Private Operations
    ///@{
    ///@}
    ///@name Serialization
    ///@{

    friend class Serializer;

    void save(Serializer& rSerializer) const override
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, GeometricalObject );
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Flags );
        rSerializer.save("Data", mData);
        rSerializer.save("Properties", mpProperties);
    }

    void load(Serializer& rSerializer) override
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, GeometricalObject );
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Flags );
        rSerializer.load("Data", mData);
        rSerializer.load("Properties", mpProperties);
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

}; // Class Element

///@}
///@name Type Definitions
///@{
///@}
///@name Input and output
///@{

/// input stream function
inline std::istream & operator >>(std::istream& rIStream,
                                  Element& rThis);

/// output stream function

inline std::ostream & operator <<(std::ostream& rOStream,
                                  const Element& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << " : " << std::endl;
    rThis.PrintData(rOStream);
    return rOStream;
}
///@}

template class KRATOS_API(KRATOS_CORE) KratosComponents<Element >;

void KRATOS_API(KRATOS_CORE) AddKratosComponent(std::string const& Name, Element const& ThisComponent);

/**
 * definition of elemental specific variables
 */

#undef  KRATOS_EXPORT_MACRO
#define KRATOS_EXPORT_MACRO KRATOS_API

KRATOS_DEFINE_VARIABLE(WeakPointerVector< Element >, NEIGHBOUR_ELEMENTS)

#undef  KRATOS_EXPORT_MACRO
#define KRATOS_EXPORT_MACRO KRATOS_NO_EXPORT

} // namespace Kratos.
#endif // KRATOS_ELEMENT_H_INCLUDED  defined

