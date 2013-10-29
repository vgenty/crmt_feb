{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Oct 29 15:14:45 2013) by ROOT version5.34/10
   TCanvas *c1 = new TCanvas("c1", "c1",10,32,700,500);
   gStyle->SetOptStat(0);
   c1->SetHighLightColor(2);
   c1->Range(-1.973684,-7.0875,1.973684,51.975);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetLeftMargin(0.12);
   c1->SetRightMargin(0.12);
   c1->SetTopMargin(0.08);
   c1->SetBottomMargin(0.12);
   c1->SetFrameLineWidth(2);
   c1->SetFrameBorderMode(0);
   c1->SetFrameLineWidth(2);
   c1->SetFrameBorderMode(0);
   
   TH1D *angles = new TH1D("angles","",25,-1.5,1.5);
   angles->SetBinContent(2,1);
   angles->SetBinContent(3,2);
   angles->SetBinContent(5,7);
   angles->SetBinContent(6,1);
   angles->SetBinContent(7,5);
   angles->SetBinContent(8,3);
   angles->SetBinContent(9,6);
   angles->SetBinContent(10,8);
   angles->SetBinContent(11,21);
   angles->SetBinContent(12,42);
   angles->SetBinContent(13,45);
   angles->SetBinContent(14,24);
   angles->SetBinContent(15,21);
   angles->SetBinContent(16,14);
   angles->SetBinContent(17,7);
   angles->SetBinContent(18,4);
   angles->SetBinContent(19,5);
   angles->SetBinContent(20,3);
   angles->SetBinContent(21,1);
   angles->SetBinContent(23,1);
   angles->SetBinContent(24,1);
   angles->SetEntries(222);
   angles->SetStats(0);
   angles->SetFillColor(2);
   angles->SetFillStyle(3002);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   angles->SetLineColor(ci);
   angles->SetLineWidth(2);
   angles->GetXaxis()->SetTitle("Angle (rad)");
   angles->GetXaxis()->SetRange(1,50);
   angles->GetXaxis()->CenterTitle(true);
   angles->GetXaxis()->SetLabelFont(63);
   angles->GetXaxis()->SetLabelSize(25);
   angles->GetXaxis()->SetTitleSize(25);
   angles->GetXaxis()->SetTitleOffset(1.05);
   angles->GetXaxis()->SetTitleFont(63);
   angles->GetYaxis()->SetTitle(" Count/0.12 rad");
   angles->GetYaxis()->CenterTitle(true);
   angles->GetYaxis()->SetLabelFont(63);
   angles->GetYaxis()->SetLabelSize(25);
   angles->GetYaxis()->SetTitleSize(25);
   angles->GetYaxis()->SetTitleOffset(1.02);
   angles->GetYaxis()->SetTitleFont(63);
   angles->GetZaxis()->SetLabelFont(42);
   angles->GetZaxis()->SetLabelSize(0.035);
   angles->GetZaxis()->SetTitleSize(0.035);
   angles->GetZaxis()->SetTitleFont(42);
   angles->Draw("");
   
   TPaveText *pt = new TPaveText(0.35,0.94,0.66,0.98,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextFont(63);
   pt->SetTextSize(27);
   TText *text = pt->AddText("Reconstructed Angle from Zenith");
   pt->Draw();
   
   pt = new TPaveText(0.5718391,0.7182203,0.8706897,0.8177966,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextFont(63);
   pt->SetTextSize(24);
   text = pt->AddText("Mean: -0.019789");
   pt->Draw();
   
   pt = new TPaveText(0.5402299,0.7881356,0.8390805,0.8877119,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextFont(63);
   pt->SetTextSize(24);
   text = pt->AddText("Entries: 222");
   pt->Draw();
   
   pt = new TPaveText(0.5632184,0.6461864,0.862069,0.7457627,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextFont(63);
   pt->SetTextSize(24);
   text = pt->AddText("RMS: 0.393430");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
