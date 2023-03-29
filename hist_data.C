#include <vector>
#include <string>

const std::string path_name = "bmn_hist/";
const std::string extension = ".png";

class hist_data
{
public:
	hist_data(string name, int n_bins, std::vector<double> x_user_range, ROOT::RDataFrame& d_fr) :
		title(name), bin(n_bins), X_Range(x_user_range) {
		p = d_fr.Histo1D({ name.c_str(), name.c_str(), bin, X_Range[0], X_Range[1] }, name); // Вот тут как можно передать datafr?
	}
	string title;
	int bin;
	std::vector<double> X_Range;
	ROOT::RDF::RResultPtr<::TH1D> p;
	void Draw_png(TCanvas* c)
	{
		p->Draw();
		c->SaveAs((path_name + title + extension).c_str());
	}
};