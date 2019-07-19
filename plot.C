void plot() {
  TFile *file = TFile::Open("AnalysisResults.root");
  TList *list = (TList*) gFile->Get("coutput1");

  TH1F *plot1 = (TH1F*) list->FindObject("fHistPt");

  TCanvas* c1 = new TCanvas("can1", "can1", 1200, 800);
  if(plot1){
    plot1->DrawCopy();

    c1->SaveAs("plot_pt.pdf");
  }

  TH1F *plot2 = (TH1F*) list->FindObject("fHistEta");

  TCanvas* c2 = new TCanvas("can2", "can2", 1200, 800);
  if(plot1){
    plot2->DrawCopy();

    c2->SaveAs("plot_eta.pdf");
  }

}
