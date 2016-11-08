/*=========================================================================

 Program: FEMuS
 Module: Marker
 Authors: Eugenio Aulisa and Giacomo Capodaglio

 Copyright (c) FEMuS
 All rights reserved.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __femus_ism_Marker_hpp__
#define __femus_ism_Marker_hpp__

//----------------------------------------------------------------------------
// includes :
//----------------------------------------------------------------------------
#include "MarkerTypeEnum.hpp"
#include "ParallelObject.hpp"
#include "Mesh.hpp"

#include "vector"
#include "map"
#include "Mesh.hpp"

namespace femus {

  class Marker : public ParallelObject {
    public:
      Marker(std::vector < double > x, const MarkerType &markerType, Mesh *mesh, const unsigned & solType, const bool &debug = false) {
        _x = x;
        _markerType = markerType;
        _mesh = mesh;
        _solType = solType;

        GetElement(1, UINT_MAX);

        if(_iproc == _mproc) {
	  FindLocalCoordinates(_solType, _aX, true);
	}
      };


      unsigned GetMarkerElement(){
	return _elem;
      }
      
      std::vector<double> GetMarkerLocalCoordinates(){
	return _xi;
      }

      void GetElement(const bool &useInitialSearch, const unsigned &initialElem);
      void GetElementSerial(unsigned &initialElem);
      void GetElement();
      

      MarkerType GetMarkerType() {
        return _markerType;
      };

      void InverseMappingTEST(std::vector< double > &x);
      void Advection(Solution* sol, const unsigned &n, const double& T);

     void updateVelocity(std::vector <double> & V, Solution* sol, 
			 const vector < unsigned > &solVIndex, const unsigned & solVType,
                         std::vector < std::vector < double > > &a,  std::vector < double > &phi, const bool & pcElemUpdate);
     
      void FindLocalCoordinates(const unsigned & solVType, std::vector < std::vector < std::vector < double > > > &aX, 
		    const bool & pcElemUpdate);

      void ProjectVelocityCoefficients(Solution * sol, const std::vector<unsigned> &solVIndex, 
				       const unsigned &solVType,  const unsigned &nDofsV,
				       const unsigned &ielType, std::vector < std::vector < double > > &a);

      
    private:


      std::vector< double > InverseMapping(const unsigned &currentElem, const unsigned &solutionType, const std::vector< double > &x);
      void InverseMapping(const unsigned &iel, const unsigned &solType,
                          const std::vector< double > &x, std::vector< double > &xi);

      unsigned GetNextElement2D(const unsigned &iel);
      unsigned GetNextElement3D(const unsigned &iel);
      int FastForward(const unsigned &currentElem);

      std::vector < double > _x;
      std::vector < double > _x0;
      std::vector < double > _xi;
      unsigned _solType;
      MarkerType _markerType;
      const Mesh * _mesh;
      unsigned _elem;

      unsigned _mproc; //processor who has the marker

      static const double _localCentralNode[6][3];
      
      std::vector < std::vector < std::vector < double > > > _aX;
      
      std::vector < std::vector < double > > _K;
            
      static const double _a[4][4][4];
      static const double _b[4][4];
      static const double _c[4][4];

  };
} //end namespace femus



#endif
