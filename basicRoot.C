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
    auto fileName = "./student-mat-signal.csv";
    auto fileName2 = "./student-mat-background.csv";
    auto rdf = ROOT::RDF::MakeCsvDataFrame(fileName, true, ',');
    auto rdf2 = ROOT::RDF::MakeCsvDataFrame(fileName2, true, ',');
    rdf.Snapshot("signalTree", "signalMathScores.root");
    TFile* signalFile = TFile::Open("signalMathScores.root");
    TTree *signalTree = signalFile->Get<TTree>("signalTree");
    rdf2.Snapshot("backgroundTree", "backgroundMathScores.root");
    TFile* backgroundFile = TFile::Open("backgroundMathScores.root");
    TTree *backgroundTree = backgroundFile->Get<TTree>("backgroundTree");
    
    TString outfileName("MathScore.root");
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                                 "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

    dataloader->AddVariable( "G1" );
    dataloader->AddVariable( "absences" );
    dataloader->AddVariable( "freetime" );

    Double_t signalWeight = 1.0;
    Double_t backgroundWeight = 1.0;
    dataloader->AddSignalTree(signalTree, signalWeight );
    dataloader->AddBackgroundTree( backgroundTree, backgroundWeight );
    dataloader->PrepareTrainingAndTestTree("", "", "SplitMode=Random:!V" );

    //Cuts and ML method
    factory->BookMethod( dataloader, TMVA::Types::kCuts, "Cuts",
                           "!H:!V:FitMethod=MC:EffSel:VarProp=FSmart" );
    factory->BookMethod( dataloader, TMVA::Types::kCuts, "CutsD",
                           "!H:!V:FitMethod=MC:EffSel:VarProp=FSmart:VarTransform=Decorrelate" );
    factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLPBNN", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=60:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS:UseRegulator" );

    //Show trends
    // factory->BookMethod( dataloader, TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
    // factory->BookMethod( dataloader, TMVA::Types::kRuleFit, "RuleFit",
    //                        "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

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