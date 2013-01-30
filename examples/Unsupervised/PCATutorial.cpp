//===========================================================================
/*!
 *  \brief Principal Component Analysis Tutorial Sample Code
 *
 *  This file is part of the "Principal Component Analysis" tutorial.
 *  The tutorial requires that you download the Cambridge Face Database
 *  from http://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html
 *  and adjust the facedirectory path to the directory containing the faces
 *  in PGM format.
 *  
 *  You need the libraries boost_serialization, boost_system, and 
 *  boost_filesystem for this example.
 *
 *  \author C. Igel
 *  \date 2011
 *
 *  <BR><HR>
 *  This file is part of Shark. This library is free software;
 *  you can redistribute it and/or modify it under the terms of the
 *  GNU General Public License as published by the Free Software
 *  Foundation; either version 3, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 */
//===========================================================================
#include <iostream>
#include <shark/Algorithms/Trainers/PCA.h>
#include <shark/Data/Pgm.h>

#include <boost/format.hpp>

using namespace std;
using namespace shark;


int main(){
	// read image data
	const char *facedirectory = "Cambridge_FaceDB"; //< set this to the directory containing the face database
	UnlabeledData<RealVector> images;
	Data<ImageInformation> imagesInfo;
	cout << "Read images ... " << flush;
	// 
	try {
		importPGMSet(facedirectory, images, imagesInfo);
	} catch(...) {
		cerr << "[PCATutorial] could not open face database directory\n\nThis file is part of the \"Principal Component Analysis\" tutorial.\nThe tutorial requires that you download the Cambridge Face Database\nfrom http://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html\nand adjust the facedirectory path in the source code to the directory\ncontaining the faces in PGM format." << endl;
		exit(EXIT_FAILURE);
	}
	cout << "done." << endl;

	unsigned l = images.size();   // number of samples
	unsigned x = imagesInfo(0).x; // width of images
	unsigned y = imagesInfo(0).y; // height of images

	cout << "Eigenvalue decomposition ... " << flush;
	PCA pca(images);
	cout << "done." << endl;
	
	cout << "Writing mean face and eigenvalues... " << flush;
	ofstream ofs("facesEigenvalues.csv");
	for(unsigned i=0; i<l; i++) ofs << pca.eigenvalue(i) << endl;
	ofs.close();
	exportPGM("facesMean.pgm", pca.mean(), x, y);
	cout << "done. " << endl;

	cout << "Encoding ... " << flush;
	unsigned m = 299;
	LinearModel<> enc;
	pca.encoder(enc, m);
	Data<RealVector> encodedImages = enc(images);
	cout << "done. " << endl;

	unsigned sampleImage = 0;
	cout << "Reconstructing face " << sampleImage << " ... " << flush;
	boost::format fmterTrue("face%d.pgm");
	boost::format fmterRec("facesReconstruction%d-%d.pgm");
	exportPGM((fmterTrue % sampleImage).str().c_str(), images(sampleImage), x, y);
	LinearModel<> dec;
	pca.decoder(dec, m);
	exportPGM((fmterRec % sampleImage % m).str().c_str(), dec(encodedImages(sampleImage)), x, y); 
	cout << "done." << endl;
}
