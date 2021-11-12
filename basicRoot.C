#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
 
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
 
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

int basicRoot()
{
    TMVA::Tools::Instance();
    auto fileName = "./student-mat-formatted.csv";
    auto rdf = ROOT::RDF::MakeCsvDataFrame(fileName, true, ',');
    rdf.Snapshot("mathScores", "mathScores.root");
    TFile* mathScoresFile = TFile::Open("mathScores.root");
    TTree *originalTree = mathScoresFile->Get<TTree>("mathScores");
    TTree *signalTree = originalTree->CopyTree("G3>=10.8");
    TTree *backgroundTree = originalTree->CopyTree("G3<10.8");
    
    TString outfileName("MathScoreResults.root");
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                                 "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

    dataloader->AddVariable( "studytime", "Study Time", "units", 'F' );
    dataloader->AddVariable( "failures", "Past Failures", "units", 'F' );
    dataloader->AddVariable( "health", "Health", "units", 'F' );
    dataloader->AddVariable( "absences", "Absences", "counts", 'F' );
    dataloader->AddVariable( "school", "School", "school", 'F' );
    dataloader->AddVariable( "G1", "First Period Grade", "score", 'F' );

    Double_t signalWeight = 1.0;
    Double_t backgroundWeight = 1.0;
    dataloader->AddSignalTree(signalTree, signalWeight );
    dataloader->AddBackgroundTree( backgroundTree, backgroundWeight );
    dataloader->PrepareTrainingAndTestTree("", "", "SplitMode=Random:!V" );

    //MLP
    factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=60:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" );

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    outputFile->Close();

    delete factory;
    delete dataloader;
    if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

    return 0;
}

int main(){
    basicRoot();
}