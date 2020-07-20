/*!
 * \file lax.hpp
 * \brief Declaration of numerics classes for Lax centered scheme. The implementation is in lax.cpp.
 * \author F. Palacios, T. Economon
 * \version 7.0.5 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2020, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../../CNumerics.hpp"

/*!
 * \class CCentLax_NEMO
 * \brief Class for computing the Lax-Friedrich centered scheme.
 * \ingroup ConvDiscr
 * \author F. Palacios.
 * \version 2.0.6
 */
class CCentLax_NEMO : public CNumerics {
private:
  unsigned short iDim, iVar, jVar; /*!< \brief Iteration on dimension and variables. */
  su2double *Diff_U; /*!< \brief Difference of conservative variables. */
  su2double *MeanU, *MeanV;
  vector<su2double> mean_eves;
  su2double *MeandPdU;
  su2double *ProjFlux;  /*!< \brief Projected inviscid flux tensor. */
  su2double Param_p, Param_Kappa_0; /*!< \brief Artificial dissipation parameters. */
  su2double Local_Lambda_i, Local_Lambda_j, MeanLambda; /*!< \brief Local eigenvalues. */
  su2double Phi_i, Phi_j, sc0, StretchingFactor; /*!< \brief Streching parameters. */
  su2double Epsilon_0, cte; /*!< \brief Artificial dissipation values. */
  //    su2double *dPdrhos, dPdrhoE, dPdrhoEve; /*!< \brief Partial derivative of pressure w.r.t. conserved quantities. */
  bool implicit; /*!< \brief Implicit time integration. */
  bool ionization;  /*!< \brief Charged species with the mixture. */
  bool stretching;
  unsigned short nPrimVar, nPrimVarGrad;

public:

	/*!
	 * \brief Constructor of the class.
	 * \param[in] val_nDim - Number of dimension of the problem.
	 * \param[in] val_nVar - Number of variables of the problem.
	 * \param[in] config - Definition of the particular problem.
	 */
	CCentLax_NEMO(unsigned short val_nDim, unsigned short val_nVar,
                unsigned short val_nPrimVar, unsigned short val_nPrimVarGrad,
                CConfig *config);

	/*!
	 * \brief Destructor of the class.
	 */
	~CCentLax_NEMO(void);

	/*!
	 * \brief Compute the flow residual using a Lax method.
	 * \param[out] val_resconv - Pointer to the convective residual.
	 * \param[out] val_resvisc - Pointer to the artificial viscosity residual.
	 * \param[out] val_Jacobian_i - Jacobian of the numerical method at node i (implicit computation).
	 * \param[out] val_Jacobian_j - Jacobian of the numerical method at node j (implicit computation).
	 * \param[in] config - Definition of the particular problem.
	 */
	void ComputeResidual(su2double *val_resconv, su2double *val_resvisc, su2double **val_Jacobian_i, su2double **val_Jacobian_j,
                         CConfig *config);
};