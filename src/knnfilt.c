// 
// ******************************************************
// KNN filtering for Analyze images
// (C) Jussi Tohka, 2005 - 2009 
// Laboratory of NeuroImaging, Department of Neurology, UCLA Medical School, CA, USA
// Institute of Signal Processing, Tampere University of Technology, Finland  
// Version 2.0.
// e-mail jussi.tohka@tut.fi
// *****************************************************
// ******************************************************
// Permission to use, copy, modify, and distribute this software 
// for any purpose and without fee is hereby
// granted, provided that the above copyright notice appears in all
// copies.  The author, Tampere University of Technology and University of
// California, Los Angeles make no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.
// *****************************************************
// This file belongs to SVPASEG package. The SVPASEG implements 
// a flexible pipeline to tissue classify MR-images. The most important 
// references are:
// [1]  J. Tohka , E. Krestyannikov, I.D. Dinov , A. MacKenzie-Graham,
// D.W. Shattuck , U. Ruotsalainen, and A.W. Toga. 
// Genetic algorithms for finite mixture model based voxel classification
// in neuroimaging.  
// IEEE Transactions on Medical Imaging  , 26(5):696 - 711, 2007.
//
// [2] J. Tohka , I.D. Dinov, D.W. Shattuck, and A.W. Toga. Brain MRI 
// Tissue Classification Based on Local Markov Random Fields, 
// Magnetic Resonance Imaging , in press, 2009. 
// ********************************************************************

#include "analyze.h"

int main(int argc,char** argv)
{
  AnalyzeImage img;
  AnalyzeLabelImage mask;
  int intstatus;  
  int i;

  bool clobber = true;
  bool status;
  bool signedData = true;

  if(argc > 2) {
    if(!(strcmp(argv[1],"-unsigned"))) {
      signedData = false;
      argc--;
      for(i = 1;i < argc;i++) {
        argv[i] = argv[i + 1];
      }
    }
  }
  if(argc < 4) {
    cout << "Usage: knnfilt [-unsigned] inputfile outputfile brainmask" << endl;
    cout <<  "If you say 'default' for brainmask, it is assumed that all non zero intensities in" << endl;
    cout <<  "input image are within the brain mask" << endl; 
    return(1);
  }
  intstatus = readImage(argv[1],&img,signedData);
  if(intstatus != 0) {
    cout << "Could not read the file" << argv[1] << endl;
    return(2);
  }
  if(!(strcmp(argv[2],"default"))) {   
    binaryMask(&img,&mask,0.0001);
  }
  else {
    intstatus = readLabelImage(argv[3],&mask);
    if(intstatus != 0) {
      cout << "Could not read image (brainmask) file " << argv[3] << " " << intstatus << endl;
      return(3);
    }  
  }
  knnFilter(&img,&mask,1,1,1,6);
  intstatus = writeImage(argv[2],&img,clobber,signedData);
  if(intstatus != 0) {
    cout << "Could not write the file" << argv[2] << endl;
    return(3);
  }
  return(0);
}
