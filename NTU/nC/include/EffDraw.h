class SpectrumCut{

    public:
        SpectrumCut(TFile* infile, TString& name, vector<TString>& v, TString& name2);
        ~SpectrumCut(){}
        bool SetCut(double cut);
        bool SetColor(vector<int>& color){ this->color = color; return true;}
        bool SetNames(vector<TString>& name){ this->name = name; return true;}
        bool SetXTitle(TString& x_title){ this->x_title = x_title; return true;}
        bool SetYTitle(TString& y_title){ this->y_title = y_title; return true;}
        bool Rebin(int a);
        bool Normalize();
        void Draw(TCanvas* c, bool Log);
        void Draw(TCanvas* c, bool Log, double xmin, double xmax);



    private:
        vector<TH1D*> h;
        vector<int> color;
        vector<TString> name;
        TString x_title;
        TString y_title;
        TLine* line;
        TText* text;
        int maxposition;
        double maximum;

};


SpectrumCut::SpectrumCut(TFile* infile, TString& name1, vector<TString>& v, TString& name2){

    this->maximum = 0;
    this->maxposition = -1;
    for(auto i : v){
        TH1D* temp = dynamic_cast<TH1D*>(infile->Get(name1 + i +name2));
        this->h.push_back(temp);
        double tempmax = temp->GetBinContent(temp->GetMaximumBin());
        if(tempmax > this->maximum){
            this->maximum = tempmax;
            this->maxposition = h.size()-1;
        }
    }


    this->name = v;


}



bool SpectrumCut::SetCut(double cut){

    this->line = new TLine(cut, 0.5, cut, 1.9*maximum);
    this->line->SetLineColor(1);
    this->line->SetLineWidth(5);


    /*this->text = new TText(cut, 1.5*.9*maximum, " Cut");
    this->text->SetTextColor(1);
    this->text->SetTextSize(0.07);
    this->text->SetTextFont(132);
    this->text->SetTextAlign(13);*/


    return true;



}




bool SpectrumCut::Rebin(int a){

    for(auto i : h) i->Rebin(a);
    maximum *= a;


    return true;


}




bool SpectrumCut::Normalize(){

    const double normal_const = 1./h.at(0)->GetEntries();
    for(auto i : h) i->Scale(1./i->GetEntries());
    line->SetY1(1.1e-6);
    line->SetY2(normal_const * line->GetY2());
    //if(text) text->SetY(normal_const * text->GetY());


    return true;


}




void SpectrumCut::Draw(TCanvas* c, bool Log){


    if(h.size() > color.size()){
        cerr << "Set color first!" << endl;
        return 0;
    }


    c->cd();
    if(Log) gPad->SetLogy(1);
    for(int i = 0; i < h.size(); i++){

        h.at(i)->GetXaxis()->CenterTitle(kTRUE);
        h.at(i)->GetYaxis()->CenterTitle(kTRUE);
        h.at(i)->SetLineWidth(5);
        h.at(i)->SetLineColor(color.at(i));
        h.at(i)->SetMarkerStyle(20);
        h.at(i)->SetMarkerSize(0.8);
        h.at(i)->SetMarkerColor(color.at(i));
        h.at(i)->SetTitle(name.at(i));
        if(not x_title.IsNull()) h.at(i)->GetXaxis()->SetTitle(x_title);
        if(not y_title.IsNull()) h.at(i)->GetYaxis()->SetTitle(y_title);
        h.at(i)->Draw("hist same");

    }
    

    c->BuildLegend();
    if(line) line->Draw("same");
    //if(text) text->Draw("same");


}





void SpectrumCut::Draw(TCanvas* c, bool Log, double xmin, double xmax){


    if(h.size() > color.size()){
        cerr << "Set color first!" << endl;
        return 0;
    }


    c->cd();
    if(Log) gPad->SetLogy(1);
    for(int i = 0; i < h.size(); i++){

        h.at(i)->GetXaxis()->CenterTitle(kTRUE);
        h.at(i)->GetYaxis()->CenterTitle(kTRUE);
        h.at(i)->SetLineWidth(5);
        h.at(i)->SetLineColor(color.at(i));
        h.at(i)->SetMarkerStyle(20);
        h.at(i)->SetMarkerSize(0.8);
        h.at(i)->SetMarkerColor(color.at(i));
        h.at(i)->SetTitle(name.at(i));
        if(not x_title.IsNull()) h.at(i)->GetXaxis()->SetTitle(x_title);
        if(not y_title.IsNull()) h.at(i)->GetYaxis()->SetTitle(y_title);
        h.at(i)->GetXaxis()->SetRangeUser(xmin, xmax);
        h.at(i)->Draw("hist same");

    }
    

    c->BuildLegend();
    if(line) line->Draw("same");
    //if(text) text->Draw("same");


}





