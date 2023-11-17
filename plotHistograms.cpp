#include <TApplication.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TKey.h>
#include <TObject.h>
#include <TString.h>
#include <TTree.h>
#include <iostream>

////////// Macros en root para generar grilla de histogramas
///////// Uso:
///////// root --web=off
///////// .x plotHistograms.cpp

const TString inputFilename = "ECAL_sim.root"; // Cambiar al archivo root de lectura
const char* outputFilename = "output.pdf";      // output 

void plotHistograms(const char* filename = inputFilename, const char* outputFilename = "output.pdf") { //quiza da problemas si no le pones el nombre del output aca 
 

  // Abrir archivo ROOT
  TFile* file = new TFile(filename, "READ");
  if (file->IsZombie()) {
    printf("Error opening ROOT file\n"); // check for zombie file
    return;
  }

  // Access the TTree
  TTree* tree = dynamic_cast<TTree*>(file->Get("ecalTree;7")); // esta hardcoded aca el tree que necesitas, cambiar el nombre
  if (!tree) {
    std::cerr << "Error: TTree 'ecalTree;7' not found in the input file." << std::endl;
    file->Close();
    delete file;
    return;
  }

  // Create a canvas for the grid
  TCanvas cellCanvas("cellCanvas", "ECAL Histograms", 1200, 1200); // Adjust the canvas size as needed
  cellCanvas.Divide(6, 6); // en este caso son 6 x 6, cambiar dependiendo del caso

  // Loop through the branches and plot them
  for (int x = 0; x < 6; ++x) { //nuevamente cambiar dependiendo del tamanho de la matriz
    for (int y = 0; y < 6; ++y) {
      cellCanvas.cd(6 * x + y + 1);

      // ajustes del pad
      gPad->SetLeftMargin(0);
      gPad->SetRightMargin(0);
      gPad->SetTopMargin(0);
      gPad->SetBottomMargin(0);
      gPad->SetBorderMode(0);
      gPad->SetFillColor(0);
      gPad->SetGrid();
      // Disable grid lines
      gPad->SetGrid(0, 0);

      TString branchName = TString::Format("%d_%d", x, y); // aca esta el nombre de los branches, en mi caso tienen nombre x_y, cambiar dependiendo del caso 

      // Create a histogram from the branch
      TH1F* hist = new TH1F(branchName, branchName, 100, 0, 20); // Rango de cada histograma 
      TString drawExpression = TString::Format("%d_%d", x, y); // cambiar segun nombre de branches

      tree->Draw(drawExpression + ">>" + branchName); 

      hist->Draw();
    }
  }

  // Guardar en archivo pdf
  cellCanvas.Print(outputFilename);

  // Cerrar  archivo 
  file->Close();
  delete file;

 
}
