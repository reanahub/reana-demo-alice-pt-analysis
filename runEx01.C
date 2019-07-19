// run.C
//
// Template run macro for AliBasicTask.cxx/.h with example layout of
// physics selections and options, in macro and task.
//
// Author: Arvinder Palaha
//
class AliAnalysisGrid;

//______________________________________________________________________________
void runEx01()
{
    // load libraries
    gSystem->Load("libCore.so");
    gSystem->Load("libGeom.so");
    gSystem->Load("libVMC.so");
    gSystem->Load("libPhysics.so");
    gSystem->Load("libTree.so");
    gSystem->Load("libMinuit.so");
    gSystem->Load("libProof.so");
    gSystem->Load("libSTEERBase.so");
    gSystem->Load("libESD.so");
    gSystem->Load("libAOD.so");
    gSystem->Load("libCDB.so");
    gSystem->Load("libANALYSIS.so");
    gSystem->Load("libOADB.so");
    gSystem->Load("libANALYSISalice.so");
    gSystem->Load("libRAWDatabase.so");
    gSystem->Load("libRAWDatarec.so");
    gSystem->Load("libSTEER.so");
    gSystem->Load("libHLTbase.so");
    gSystem->Load("libTRDbase.so");
    gSystem->Load("libTPCbase.so");
    gSystem->Load("libTPCrec.so");

    // add aliroot indlude path
    gROOT->ProcessLine(Form(".include %s/include",gSystem->ExpandPathName("$ALICE_ROOT")));
    gROOT->SetStyle("Plain");

    // analysis manager
    AliAnalysisManager* mgr = new AliAnalysisManager("Example01");
    // create the alien handler and attach it to the manager
    AliAnalysisGrid *plugin = CreateAlienHandler();
    mgr->SetGridHandler(plugin);

    AliVEventHandler* iH = new AliESDInputHandler();
//    AliAODInputHandler* iH = new AliAODInputHandler();
//    iH->SetInactiveBranches("tracks. vertices. v0s. cascades. jets. caloClusters. fmdClusters. pmdClusters. dimuons. AliAODZDC");
//    iH->SetInactiveBranches("*");
//    iH->SetCheckStatistics(kTRUE);
    mgr->SetInputEventHandler(iH);

    // create task
    gROOT->LoadMacro("AliAnalysisTaskEx01.cxx+g");
    AliAnalysisTaskSE* task = new AliAnalysisTaskEx01("taskpt");
    task->SelectCollisionCandidates(AliVEvent::kMB); // if physics selection performed in UserExec(), this line should be commented
    mgr->AddTask(task);

    // set output root file name for different analysis
    TString outfilename = "AnalysisResults.root";

    // create containers for input/output
    AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
    AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("coutput1", TList::Class(), AliAnalysisManager::kOutputContainer, outfilename);

    // connect input/output
    mgr->ConnectInput(task, 0, cinput);
    mgr->ConnectOutput(task, 1, coutput1);

    // enable debug printouts
    mgr->SetDebugLevel(1);
//    mgr->SetNSysInfo(100);
    if (!mgr->InitAnalysis()) return;
    mgr->PrintStatus();
//  Mag field
    AliCDBManager *cdb = AliCDBManager::Instance();
    cdb->SetDefaultStorage("local://");
    cdb->SetRun(0);
    cdb->InitFromSnapshot("OCDB.root");

    AliGRPManager *grp= new AliGRPManager();
    printf("#### Loading GRP to init B-field...\n");
    if(!grp->ReadGRPEntry()) AliFatal("Cannot get GRP entry");
    if(!grp->SetMagField())  AliFatal("Problem with magnetic field setup");

    // start analysis
    Printf("Starting Analysis....");
    mgr->StartAnalysis("local");
}

//______________________________________________________________________________
AliAnalysisGrid* CreateAlienHandler()
{
    AliAnalysisAlien *plugin = new AliAnalysisAlien();
    // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
    plugin->SetRunMode("test");

    // Plugin test mode works only providing a file containing test file locations, used in "local" mode also
    plugin->SetFileForTestMode("data.txt"); // file should contain path name to a local directory containg *ESDs.root etc
    return plugin;
}

