#include "TFile.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TTree.h"
#include "../goddess/include/GoddessStruct.h"
#include <iostream>
#include <fstream>

//Variables used in GoddessAnalysis.cxx are initialized here.	
	TTree *nt;
	TFile *data;
	TFile *hist;

	vector<GamData> *gam2;
	vector<SiDataBase> *si2;

	TDirectory *Particle_IDs;
	TDirectory *D4_FB_PIDs;
	TDirectory *D5_FB_PIDs;
	TDirectory *D7_FB_PIDs;
	TDirectory *D8_FB_PIDs;
	TDirectory *D9_FB_PIDs;
	TDirectory *D10_FB_PIDs;

	TDirectory *Calibration_Hists;
	TDirectory *Triton_Spectra;
	TDirectory *Gamma_Hists;
	TDirectory *Strip_vs_E_Hists;

	TDirectory *QQQ5_DA_PID;
	TDirectory *QQQ5_DA_PID_Cal;
	TDirectory *QQQ5_DC_PID;

	TDirectory *DA_Triton_Spectra;
	TDirectory *DC_Triton_Spectra;
	TDirectory *QQQ5_DA_dE_gated;

	TDirectory *Gammasphere_Hists;
	TDirectory *QQQ5_DA_TotalE;
	TDirectory *Gamma_Gated_Hists;
	TDirectory *Gamma_Intensity_Hists;
	TDirectory *Q_Gated_QQQ_DA_E1;
	TDirectory *Q_Gated_QQQ_DA_dE;

	TDirectory *Timing_Spectra;

	TDirectory *Excitation_Energy_Gated;

	TH1D *E1proj;
	TH2D *dEE1check;

	TH2D *QQQDAdE;
	TH2D *QQQDAE1;
	TH2D *QQQDAE2;
	TH2D *QQQDAE1sum;
	TH2D *QQQDAE1cal;
	TH2D *QQQDAdEcal;
	TH2D *QQQDATot;
	TH2D *QQQDATot2;
	TH2D *QQQDAQ;
	TH2D *QQQDAQAl;
	TH1D *QQQDATotQ;
	TH2D *QQQDAEx;
	TH1D *QQQDAExTot;

	TH1D *QQQDAExtotTiming;

	TH2D *QQQDCdE;
	TH2D *QQQDCE1;
	//TH1D *QQQDAtot;
	TH2D *QQQDAvsGam;

	TH2D *gam_DA;
	TH2D *gam_DC;
	
	TH1D *TimeHist;
	TH2D *gamTime;
	TH1D *gam511vtime;
	TH1D *gam511vtimeBACK;
	TH1D *gam238vtime;
	TH1D *gam238vtimeBACK;
	TH1D *gam1233vtime;
	TH1D *gam1233vtimeBACK;
	TH1D *gam1633vtime;
	TH1D *gam1633vtimeBACK;


	TH1D *QQQDAdE0;

	TH2D *gam_ind;
	TH1D *gam_tot;

	std::vector<TH1D*> gam_gated;
	std::vector<TH1D*> DA_TotE;

	int nEntries;
	
	std::vector<TH2D*> DA_PIDhists;
	std::vector<TH2D*> DA_PIDhists_Cal;
	std::vector<TH2D*> DC_PIDhists;

	std::vector<TH1D*> sT_Spec;
	std::vector<std::vector<TH1D*>> DA_T_Spec;
	std::vector<std::vector<TH1D*>> DC_T_Spec;
	
	std::vector<TH1D*> dE_gated;
	std::vector<std::vector<TH1D*>> DA_dE_Gated;
	std::vector<TH1D*> Q_gated_dE;
	std::vector<TH1D*> Q_gated_E1;
	std::vector<std::vector<TH1D*>> Q_DA_dE_Gated;

	std::vector<TH2D*> PIDCont;
	std::vector<std::vector<TH2D*>> PIDCont2;
	std::vector<std::vector<std::vector<TH2D*>>> D4_PID; 
	std::vector<std::vector<std::vector<TH2D*>>> D5_PID; 
	std::vector<std::vector<std::vector<TH2D*>>> D7_PID; 
	std::vector<std::vector<std::vector<TH2D*>>> D8_PID; 
	std::vector<std::vector<std::vector<TH2D*>>> D9_PID; 
	std::vector<std::vector<std::vector<TH2D*>>> D10_PID;
 
	TH1D *gam238;
	TH1D *gam275;
	TH1D *gam1233;
	TH1D *gam1297;
	TH1D *gam1840;

	TH1D *int238;
	TH1D *back238;

	TH1D *int275;
	TH1D *back275;

	TH1D *int1232;
	TH1D *back1232;

	TH1D *int1840;
	TH1D *back1840;

	TH1D *int2556;
	TH1D *back2556;

	TH1D *int4140;
	TH1D *back4140;

	TH1D *int4362;
	TH1D *back4362;

	TH1D *TDiffDA;
	TH1D *TDiffDC;
	TH1D *TDiffSX3;

	std::vector<TH1D*> ExGatedGams;

	double AandEloss[20][6];

	//Calibrations for the DA E1 detector and DA dE detector are stored in these arrays.
	double calDAE1[32][2];
	double calDAdE[32][2];

	//normalization parameters for the DA E1 histograms. normDA0 is the slope and normDA1 is the intercept in m*x+b. The normalization parameters were found by matching dE-E strip combinations to the first useful looking dE-E strip combo.
	double normDA0[32][5];
	double normDA1[32][5];

	//normalization parameters for the DC E1 histograms. normDC0 is the squared term, normDC1 is the slope, and normDC2 is the intercept in nx^2+mx+b. The normalization parameters were found by matching dE-E strip combinations to the first useful looking dE-E strip combo.
	double normDC0[32][5];
	double normDC1[32][5];
	double normDC2[32][5];

	//Bounds for the Gamma coincidence gates. The first two columns are upper, lower bounds for Peak 0, etc. The peaks that don't have bounds for the higher strips are given a value of 1.0 in the txt file. 0 was not used in case some of the energies read 0. 
	double boundDA[32][8];

	//Q-Value bins for the gamma intensity histograms are initialized here.
	double Qbins[300];
	double Qgatearray[300][2];
	double buff;

	//Gammasphere quandratic calibration parameters stored here
	double gamcalparams[111][3];

	//QQQ DC Triton gates
	double TGateDCLow[22][5];
	double TGateDCHi[22][5];

	//PID Gates from calibrated energies
	double CalPIDGateLo[32][5];
	double CalPIDGateHi[32][5];

	TH1D *SX3_0;
	TH1D *SX3_1;
	TH1D *SX3_2;
	TH1D *SX3_3;

	double ExGateArrLow[31];
	double ExGateArrHi[31];

