/*!
 * \file CFEMStandardHexPartition.cpp
 * \brief Functions for the class CFEMStandardHexPartition.
 * \author E. van der Weide
 * \version 7.0.8 "Blackbird"
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

#include "../../include/fem/CFEMStandardHexPartition.hpp"

/*----------------------------------------------------------------------------------*/
/*             Public member functions of CFEMStandardHexPartition.                 */
/*----------------------------------------------------------------------------------*/

CFEMStandardHexPartition::CFEMStandardHexPartition(const unsigned short val_nPoly,
                                                   const unsigned short val_orderExact)
  : CFEMStandardHexBase(val_nPoly,val_orderExact) {

  /*--- Determine the 1D parametric locations of the grid DOFs. 1D is enough,
        because a tensor product is used to obtain the 3D coordinates. ---*/
  Location1DGridDOFsEquidistant(nPoly, rLineDOFsEqui);
  Location1DGridDOFsLGL(nPoly, rLineDOFsLGL);

  /*--- Compute the values of the 1D Lagrangian basis functions in the integration
        points for both the equidistant and LGL point distribution. ---*/
  LagBasisIntPointsLine(rLineDOFsEqui, rLineInt, lagBasisLineIntEqui);
  LagBasisIntPointsLine(rLineDOFsLGL,  rLineInt, lagBasisLineIntLGL);

  /*--- Compute the values of the derivatives of the 1D Lagrangian basis functions in
        the integration points for both the equidistant and LGL point distribution. ---*/
  DerLagBasisIntPointsLine(rLineDOFsEqui, rLineInt, derLagBasisLineIntEqui);
  DerLagBasisIntPointsLine(rLineDOFsLGL,  rLineInt, derLagBasisLineIntLGL);

  /*--- Create the local grid connectivities of the faces of the volume element. ---*/
  LocalGridConnFaces();

  /*--- Determine the local subconnectivity of this standard element when split
        in several linear elements. Used for a.o. plotting and searching. ---*/
  CFEMStandardHexBase::SubConnLinearElements();

  /*--- Set the function pointers for the tensor product multiplications to
        determine the data in the volume integration points. ---*/
  SetFunctionPointerVolumeDataHex(nDOFs1D, nInt1D, TensorProductDataVolIntPoints);
}

void CFEMStandardHexPartition::CoorIntPoints(const bool                LGLDistribution,
                                             ColMajorMatrix<su2double> &matCoorDOF,
                                             ColMajorMatrix<su2double> &matCoorInt) {

  /*--- Check for which point distribution the coordinates must be computed. ---*/
  if( LGLDistribution ) {

    /*--- LGL distribution. Call the function TensorProductVolumeDataHex to compute the
          Cartesian coordinates in the integration points. ---*/
   TensorProductVolumeDataHex(TensorProductDataVolIntPoints, 3, nDOFs1D, nInt1D,
                              lagBasisLineIntLGL, lagBasisLineIntLGL, lagBasisLineIntLGL,
                              matCoorDOF, matCoorInt, nullptr);
  }
  else {

    /*--- Equidistant distribution. Call the function TensorProductVolumeDataHex to compute the
          Cartesian coordinates in the integration points. ---*/
    TensorProductVolumeDataHex(TensorProductDataVolIntPoints, 3, nDOFs1D, nInt1D,
                               lagBasisLineIntEqui, lagBasisLineIntEqui, lagBasisLineIntEqui,
                               matCoorDOF, matCoorInt, nullptr);
  }
}

void CFEMStandardHexPartition::DerivativesCoorIntPoints(const bool                         LGLDistribution,
                                                        ColMajorMatrix<su2double>          &matCoor,
                                                        vector<ColMajorMatrix<su2double> > &matDerCoor) {

  /*--- Check for which point distribution the derivatives must be computed. ---*/
  if( LGLDistribution ) {

    /*--- LGL distribution. Call the function TensorProductVolumeDataHex 3 times to compute the
          derivatives of the Cartesian coordinates w.r.t. the three parametric coordinates. ---*/
    TensorProductVolumeDataHex(TensorProductDataVolIntPoints, 3, nDOFs1D, nInt1D, 
                               derLagBasisLineIntLGL, lagBasisLineIntLGL, lagBasisLineIntLGL,
                               matCoor, matDerCoor[0], nullptr);
    TensorProductVolumeDataHex(TensorProductDataVolIntPoints, 3, nDOFs1D, nInt1D,
                               lagBasisLineIntLGL, derLagBasisLineIntLGL, lagBasisLineIntLGL,
                               matCoor, matDerCoor[1], nullptr);
    TensorProductVolumeDataHex(TensorProductDataVolIntPoints, 3, nDOFs1D, nInt1D,
                               lagBasisLineIntLGL, lagBasisLineIntLGL, derLagBasisLineIntLGL,
                               matCoor, matDerCoor[2], nullptr);
  }
  else {

    /*--- Equidistant distribution. Call the function TensorProductVolumeDataHex 3 times to compute the
          derivatives of the Cartesian coordinates w.r.t. the three parametric coordinates. ---*/
    TensorProductVolumeDataHex(TensorProductDataVolIntPoints, 3, nDOFs1D, nInt1D,
                               derLagBasisLineIntEqui, lagBasisLineIntEqui, lagBasisLineIntEqui,
                               matCoor, matDerCoor[0], nullptr);
    TensorProductVolumeDataHex(TensorProductDataVolIntPoints, 3, nDOFs1D, nInt1D,
                               lagBasisLineIntEqui, derLagBasisLineIntEqui, lagBasisLineIntEqui,
                               matCoor, matDerCoor[1], nullptr);
    TensorProductVolumeDataHex(TensorProductDataVolIntPoints, 3, nDOFs1D, nInt1D,
                               lagBasisLineIntEqui, lagBasisLineIntEqui, derLagBasisLineIntEqui,
                               matCoor, matDerCoor[2], nullptr);
  }
}

passivedouble CFEMStandardHexPartition::WorkEstimateVolume(CConfig *config) {

  /*--- TEMPORARY IMPLEMENTATION. ---*/
  return nIntegration + 0.1*nDOFs;
}

/*----------------------------------------------------------------------------------*/
/*             Private member functions of CFEMStandardHexPartition.                */
/*----------------------------------------------------------------------------------*/

void CFEMStandardHexPartition::LocalGridConnFaces(void) {

  /*--- Allocate the first index of gridConnFaces, which is equal to the number
        of faces of the hexahedron, which is 6. Reserve memory for the second
        index afterwards. ---*/
  const unsigned short nDOFsQuad = (nPoly+1)*(nPoly+1);
  gridConnFaces.resize(6);

  for(unsigned short i=0; i<6; ++i) gridConnFaces[i].reserve(nDOFsQuad);

  /*--- Loop over all the nodes of the hexahedron and pick the correct
        ones for the faces. ---*/
  unsigned int ii = 0;
  for(unsigned short k=0; k<=nPoly; ++k) {
    for(unsigned short j=0; j<=nPoly; ++j) {
      for(unsigned short i=0; i<=nPoly; ++i, ++ii) {
        if(k == 0)     gridConnFaces[0].push_back(ii);
        if(k == nPoly) gridConnFaces[1].push_back(ii);
        if(j == 0)     gridConnFaces[2].push_back(ii);
        if(j == nPoly) gridConnFaces[3].push_back(ii);
        if(i == 0)     gridConnFaces[4].push_back(ii);
        if(i == nPoly) gridConnFaces[5].push_back(ii);
      }
    }
  }

  /*--- Make sure that the element is to the left of the faces. ---*/
  const unsigned short n0 = 0;
  const unsigned short n1 = nPoly;
  const unsigned short n2 = nDOFsQuad -1;
  const unsigned short n3 = n2 - nPoly;
  const unsigned short n4 = n0 + nDOFsQuad*nPoly;
  const unsigned short n5 = n1 + nDOFsQuad*nPoly;
  const unsigned short n6 = n2 + nDOFsQuad*nPoly;
  const unsigned short n7 = n3 + nDOFsQuad*nPoly;

  ChangeDirectionQuadConn(gridConnFaces[0], n0, n1, n2, n3);
  ChangeDirectionQuadConn(gridConnFaces[1], n4, n7, n6, n5);
  ChangeDirectionQuadConn(gridConnFaces[2], n0, n4, n5, n1);
  ChangeDirectionQuadConn(gridConnFaces[3], n3, n2, n6, n7);
  ChangeDirectionQuadConn(gridConnFaces[4], n0, n3, n7, n4);
  ChangeDirectionQuadConn(gridConnFaces[5], n1, n5, n6, n2);
}