#ifndef _CLASS_SMOOTHER_H_
#define _CLASS_SMOOTHER_H_

#include <stdio.h>

#include "Displacement.h"

namespace flirt {

/** \brief Abstract base for the smoother. 
    \author Stefan Heldmann, Nils Papenberg
*/
class Smoother {
public:
    Smoother() {
	//printf("Constcutor Smoother()\n\n");
        value = -1.0;
    };
    
    virtual ~Smoother(){};
    
    /** compute S[u] */
    virtual void eval(const Displacement&) = 0;

    /** compute dS[u] */
    virtual void evalGradient(const Displacement&) = 0;
    

    /** get S[u] */
    double getValue() const {
        return value;
    };

    /** get dS[u] */
    virtual const Array& getGradient() const = 0;

protected:
    double value;
};






/** \brief The diffusive smoother, 
    \f$ 
    S[u] = \frac{1}{2}\sum_{\ell=1}^d \int_{\Omega} ||\nabla u_\ell(x)||^2 \ dx\f$. 
    \author Nils Papenberg
*/
class DiffusiveSmoother : public Smoother {
public:
    DiffusiveSmoother():grad(){
    };
	
    ~DiffusiveSmoother() {
    };
	
    void eval(const Displacement& u) {
        
    };
	
    void evalGradient(const Displacement &u) {
        
    };
	
    
    const Array& getGradient() const {
        return grad;
    };
    
protected:
        Array3D grad;
    
    
};




/** \brief The curvature smoother, 
    \f$ 
    S[u] = 
    \frac{1}{2}\sum_{\ell=1}^d \int_{\Omega} (\Delta u_\ell(x))^2 \ dx
    \f$. 
    \author Stefan Heldmann
*/
class CurvatureSmoother : public Smoother {
public:
    CurvatureSmoother():grad(){
    };
	
    ~CurvatureSmoother() {
    };
	
    void eval(const Displacement& u) {
    };
	
    void evalGradient(const Displacement &u) {
    };
  
  const Array& getGradient() const {
        return grad;
  };
    
protected:
        Array3D grad;
};





/** \brief The elastic smoother, 
    \f$ 
    S[u] = 
    \frac{\mu}{2}\sum_{\ell=1}^d \int_{\Omega} ||\nabla u_\ell(x)||^2 \ dx
    + 
    \frac{\mu+\lambda}{2} \int_{\Omega} (\mathrm{div} u(x))^2 \ dx
    \f$. 
    \author Stefan Wirtz
*/
class ElasticSmoother : public Smoother {
public:
    ElasticSmoother():grad(){
    };
	
    ~ElasticSmoother() {
    };
	
    void eval(const Displacement& u) {
    };
	
    void evalGradient(const Displacement &u) {
	};
	    
    const Array& getGradient() const {
        return grad;
    };
    
protected:
        Array3D grad;
};

}

#endif
