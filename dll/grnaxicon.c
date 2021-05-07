/*
####################################################################
####################################################################

   Copyright (C) 2021 Synopsys, Inc. This Synopsys product and all associated
   documentation are proprietary to Synopsys, Inc. and may only be used 
   pursuant to the terms and conditions of a written license agreement 
   with Synopsys, Inc. All other use, reproduction, modification, or 
   distribution of the Synopsys product or the associated documentation 
   is strictly prohibited.

####################################################################
####################################################################
*/

/*
 * Purpose: Evaluates n and n*GRAD(n) of a user-defined index of
 *          refraction gradient.  The function is of the form
 *          n(x,y,z), where (x,y,z) are the cartesian coordinates
 *          of a point in the gradient.  The origin of this
 *          coordinate system is the vertex of the surface to which
 *          the gradient is attached.  GRAD(n) is the three
 *          derivatives of the function n, viz.,
 *          (dn/dx, dn/dy, dn/dz).
 *
 * Parameters:
 *   The following is a brief description of the parameters in the
 *   call list.  If the parameter is designated as "input", its
 *   value is passed to the subroutine by the calling program;  if
 *   it is designated as "output", its value is supposed to be
 *   calculated or set by this subroutine and passed back to the
 *   calling program.
 *
 *    KERROR - Error code (output): This is set to zero before this
 *             subroutine is entered.  If there are any error
 *             conditions which prevent normal completion of
 *             the calculation (eg., negative SQRT, etc.),
 *             KERROR should be set to a non-zero value.
 *
 *    BRIND  - The base index (input): This is entered in the
 *             private catalog as the index of refraction.
 *
 *    COEF   - The gradient coefficients (input): these are entered
 *             in the private catalog as the coefficients.  For
 *             example, COEF(1) is entered with the command UDG C1,
 *             COEF(2) is entered with the command UDG C2, etc.
 *
 *    S      - The position vector (input): S is an array
 *             containing the three components, (X,Y,Z), of the
 *             position of a ray as it is traced; Z is the optical
 *             axis.
 *
 *    RINDX  - The calculated index of refraction (output): This is
 *             the calculated value of the index of refraction at
 *             (X,Y,Z) using the equation programmed in this
 *             subroutine.
 *
 *    XNGRAN  - The calculated n*GRAD(n) (output): XNGRAN is an
 *              array containing the three components of the
 *              calculated values of n*GRAD(n) at (X,Y,Z) using
 *              the equation programmed in this subroutine.  Since
 *              n*GRAD(n) is equivalent to 0.5*GRAD(n**2), either
 *              form can be used, depending on which is more
 *              convenient.
 */
#include <math.h>

void grnaxicon(long* kerror, double* brind, double* coef, double* s, double* rindx, double* xngran)
{
	/* Local variables */
	float	temp, tempa, rsq;

	/*
	 * Note: The following statements defining the type of the
	 *       variables in the call list must be left in the subroutine
	 *
	 * NOTE: The index of refraction is always calculated as a
	 *       positive quantity regardless of the direction of the
	 *       light.  The Z coordinate is positive in the direction of
	 *       the material regardless of the direction of the light.
	 *
	 * NOTE: From this point on, you will typically substitute
	 *       your C code for the particular gradient being
	 *       programmed
	 */

	/*
	 * Sample calculation for Luneberg lens
	 */

	/* Parameter adjustments */
	--xngran;
	--s;
	--coef;

	/* Function Body */
	/*if (coef[1] == (float)0.) {
		*kerror = 1;
		return;
	}*/

	rsq = sqrt(s[1]*s[1] + s[2]*s[2]);
	tempa = coef[1]*(rsq - coef[2]);
	temp = 2/(exp(tempa)+exp(-tempa));

	if (temp < (float)0.) {
		*kerror = 1;
		return;
	}
	*rindx = *brind * temp;
/* 	n*GRAD(n) */

	/* Index of refraction */

    if (rsq == 0){ /* We put rsq very small but not null to avoid division by zero */  
        rsq = 0.00001;
    }

    temp = 2*coef[1]*(exp(-tempa)-exp(tempa))/((pow((exp(tempa)+exp(-tempa)),2))*rsq);
	xngran[1] = pow(*brind, 2) * s[1] * temp;
	xngran[2] = pow(*brind, 2) * s[2] * temp;
	xngran[3] = 0;
}
