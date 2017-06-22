#include "ditto_ana_flatbaby.h"

int main(int argc, char* argv[])
{
  if (argc != 3 && argc != 4)
  {
    PrintUtil::print("Usage:");
    PrintUtil::print("");
    PrintUtil::print("        ./ditto_ana_flatbaby INPUT_FILE_PATH OUTPUT.root [MAXEVENTS]  # the last three argument must be provided at the same time");
    PrintUtil::print("");
    PrintUtil::error("Please check your arguement!");
  }

  const char* input_file_path = argv[1];
  const char* output_file_path = argv[2];

  int max_nevents = -1;
  // When 4 arguments are provided
  if (argc == 4)
    max_nevents = atoi(argv[3]);

  // Determine whether this is a fastsim or not
  TString infile = input_file_path;
  LoopUtil::isfastsim = bool(infile.Contains("FSPremix") || infile.Contains("FastAsympt25ns") || infile.Contains("TChi") || infile.Contains("condor") || infile.Contains("VBF-C1N2"));

  // Determine whether this is a data run
  TString outfile = output_file_path;
  LoopUtil::isdata = bool(outfile.Contains("data_"));

  TChain *chsig = new TChain("Events");
  chsig->Add(input_file_path);
  CMS3Looper(chsig, output_file_path, max_nevents);

}
