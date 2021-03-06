//Matthew Hall
//Analysis code for 19F(3He,t)19Ne performed at Argonne National Lab 9/2015.

#include "MattAnalysis.h"

void histfill(int runnum, ofstream &log1){
	std::cout << "Run " << runnum << " is now being analyzed." << std::endl; 	
	std::cout << "../exec/rootfiles/run" + TString(std::to_string(runnum)) + ".root" << endl;
	//the code opens the file here
	data = TFile::Open("../exec/rootfiles/ReVGateTest/run" + TString(std::to_string(runnum)) + "SX3revgatetest.root");
	//trees->cd();

	//the data from trees/god is placed into a container gam2 and si2.
	//nt = (TTree*)data->Get("trees/god");
	nt = (TTree*)data->Get("god");
	gam2={};
	si2={};

	nt->SetBranchAddress("gam",&gam2);
	nt->SetBranchAddress("si",&si2);
	
	nEntries = nt->GetEntries();
	//A generic gate to use whenever I need a quick one. May be commented out much of the time.
	int gate = 0;
	int gamgate = 0;
	int Qgate = 0;
	int Exgate = 0;

	int g238 = 0; //238 keV in 19Ne
	int g275 = 0; //275 keV in 19Ne
	int g1233 = 0; //1232 keV in 19Ne
	int g1297 = 0;
	int g1840 = 0;
	int g2556 = 0;
	int g4362 = 0;

	int gtest1 = 0;
	int gtest2 = 0;

	int g196 = 0;
	int g109 = 0;
	int g890 = 0;
	int g754 = 0;

	int g511 = 0;
	int gCRAP = 0;
	

	int timegate = 0;

	//Gates for each of the peaks in the triton spectra. Gate 0 refers to the Right most peak, Gate 0 refers to the one to the left and so on...
	int pkgate[4] = {0};

	int DCgate = 0;

	//Tighter triton gates are defined here for each strip and implemented below. The Gates are drawn for the RAW DATA
	int multhiDA[32] = {36869,43521,53842,61624,49340,40212,71464,51217,95073,87486,56305,56484,52242,50244,60521,58477,1,76951,55477,1,74043,70876,77413,94147,81916,55746,101339,44402,1,1,1,1};
	double exphiDA[32] = {-.582,-.600,-.628,-.652,-.620,-.588,-.674,-.620,-.706,-.700,-.633,-.623,-.617,-.610,-.631,-.625,0,-.676,-.613,0,-.646,-.646,-.6525,-.692,-.6574,-.5995,-.6998,-.5656,0,0,0,0};

	int multloDA[32] = {48465,48904,67376,79593,57971,46821,90206,60449,168350,77873,66289,66867,59435,65073,63094,67867,1,85541,64814,1,61649,89429,1,117354,55882,64826,106899,60760,1,1,1,1};
	double exploDA[32] = {-.646,-.651,-.685,-.715,-.667,-.633,-.730,-.664,-.825,-.706,-.679,-.669,-.657,-.674,-.660,-.669,0,-.715,-.660,0,-.639,-.704,0,-.753,-.604,-.643,-.73,-.641,0,0,0,0};


	//Q-value gates here
	double Qgatelo[5] = {-3.43, -4.78, -6.146, -7.64, -8.1};
	double Qgatehi[5] = {-3.23, -4.58, -5.946, -7.44, -7.795};


	//E1 norm is defined below. It is the E1 energy multiplied by the normalization that makes all of the E1 energies lie on top of each other for all of the dE,E1 combos.
	double E1norm;
	//E1cal is the calibrated E1 energy (strips 0 through 19)
	double E1cal;
	//dEcal is the calibrated dE energy (strips 0 through 19)
	double dEcal;
	//Total Calibrated energy.
	double Ecaltot;
	//Einit is the initial triton Energy from the reaction
	double Einit;

	double dE;
	double E1;
	double E2;

	double E1N;

	int stripdE;
	int stripE1;
	int stripE2;

	int stripE1N;

	double positionE1;

	double angle;

	double PIDGateHi;
	double PIDGateLo;
	double PIDGateHiCal;
	double PIDGateLoCal;

	int sec = -1;

	double gamEn;

	int Match;

	int BadGams[19] = {7,21,26,27,28,31,35,39,47,48,50,56,62,63,68,87,98,105,110};

	double m3H = 3.016049;
	double m3He = 3.016029;
	double m19Ne = 19.001880;
	double m19F = 18.998403;
	double m27Al = 26.981539;
	double m27Si = 26.986704;
	double utoMeV = 931.4941;

	//Q-value of the reaction
	double Q = 0;
	double QAl;
	double Ex19Ne = 0;

	double tritongate = 0;

	double timediff;

	std::vector<double> gam511;

	int SX3Gate = 0;

	double pi = 3.14159265;

	//In the TimeDiff vector, index 1 refers to the Gamma and index 2 refers to the Si event
	std::vector<std::vector<double>> TimeDiff;

	std::vector<double> SiTime;

	//Saves the Gamma Index number for a time that is in the time gate
	std::vector<int> TimingGammaIndex;
	

	log1 << "Run Number: " << runnum << " Number of Entries: " << nEntries << std::endl;


	for (int evt=0;evt<nEntries;evt++){
		//Reset the gates to 0 for each entry of the tree.
		gate = 0;
		gamgate = 0;
		Qgate = 0;
		Exgate = 0;

		g238 = 0; //238 keV in 19Ne
		g275 = 0; //275 keV in 19Ne
		g1233 = 0; //1232 keV in 19Ne
		g1297 = 0;
		g1840 = 0;
		g2556 = 0;
		g4362 = 0;

		gtest1 = 0;
		gtest2 = 0;

		g196 = 0;

		g109 = 0;
		g890 = 0;
		g754 = 0;

		g511 = 0;

		gCRAP = 0;
		
		timegate = 0;

		tritongate = 0;

		int ExGates[31] = {0};

		gam511.clear();

		DCgate = 0;

		SX3Gate = 0;
				
		for (int g = 0; g<4; g++) pkgate[g] = 0;

		nt->GetEntry(evt);
		if(evt % 10000 == 0){
			cout << "Run " << runnum << ": Processing event number "<< evt << " of " << nEntries << " \r";
			std::cout.flush();
		}

		//We'll use the for loop here to draw gamma gates.

		for (unsigned int i = 0; i < gam2->size();i++){
			//if (gam2->at(i).en*.81 > 236 && gam2->at(i).en*.81 < 242) gamgate++;
			gamEn = pow(gam2->at(i).en,2)*gamcalparams[gam2->at(i).num][0] + gam2->at(i).en*gamcalparams[gam2->at(i).num][1] + gamcalparams[gam2->at(i).num][2]; 

			if (gamEn >= 235 && gamEn <= 241) g238++;
			if (gamEn >= 272 && gamEn <= 278) g275++;
			if (gamEn >= 1224 && gamEn <= 1238) g1233++;
			if (gamEn >= 1276 && gamEn <= 1314) g1297++;
			if (gamEn >= 1838 && gamEn <= 1842) g1840++;
			if (gamEn >= 2526 && gamEn <= 2576) g2556++;
			if (gamEn >= 4320 && gamEn <= 4409) g4362++;

			if (gamEn >= 241 && gamEn <= 247) gtest1++;
			//if (gamEn >= 2576 && gamEn <= 2626) gtest1++;
			if (gamEn >= 278 && gamEn <= 284) gtest2++;

			if (gamEn >= 192 && gamEn <= 200) g196++;
			if (gamEn >= 106 && gamEn <= 112) g109++;
			if (gamEn >= 886 && gamEn <= 896) g890++;
			if (gamEn >= 751 && gamEn <= 757) g754++;

			if (gamEn >= 505 && gamEn <= 515) g511++;

			if ((gamEn < 234) || (gamEn > 241 && gamEn < 271) || (gamEn > 278 && gamEn < 504) || (gamEn > 515 && gamEn < 1000)) gCRAP++;

		}


		//Fill the Si histograms here:
		for(unsigned int i = 0; i < si2->size();i++){

			dE = si2->at(i).ESumLayer(0,false);
			E1 = si2->at(i).ESumLayer(1,false);
			E2 = si2->at(i).ESumLayer(2,false);

			//Backsides used for SX3s only right now, the energy is initialized here.
			E1N = si2->at(i).ESumLayer(1,true);

			stripdE = si2->at(i).StripMaxLayer(0,false);
			stripE1 = si2->at(i).StripMaxLayer(1,false);
			stripE2 = si2->at(i).StripMaxLayer(2,false);

			stripE1N = si2->at(i).StripMaxLayer(1,true);

			positionE1 = si2->at(i).PosE1().Z();

			//rstrip is the difference between the E1 strip number and the dE strip number
			int rstrip = stripE1-stripdE;			

			//***********************************BEGIN TIMING********************************************
			for (unsigned int w = 0; w < gam2->size();w++){
				TimingGammaIndex.push_back(0);
				gamEn = pow(gam2->at(w).en,2)*gamcalparams[gam2->at(w).num][0] + gam2->at(w).en*gamcalparams[gam2->at(w).num][1] + gamcalparams[gam2->at(w).num][2]; 
				

				if (si2->at(i).telescopeID == 1100){
					timediff = (double)gam2->at(w).time - (double)si2->at(i).TimestampMaxLayer(0,false);
					//if (timediff < 445 && timediff > 400){
					if (timediff < 430 && timediff > 412){
						TimingGammaIndex[w]++;
						timegate++;

					}

					TDiffDA->Fill(timediff);
					
				}

				if (si2->at(i).telescopeID == 1102){
					timediff = (double)gam2->at(w).time - (double)si2->at(i).TimestampMaxLayer(0,false);
					if (timediff < 203 && timediff > 189){
						//TimingGammaIndex[w]++;
						timegate++;
					}
					TDiffDC->Fill(timediff);
				}
	
				if (si2->at(i).telescopeID >= 2104 && si2->at(i).telescopeID <= 2110 && si2->at(i).telescopeID != 2106){
					timediff = (double)gam2->at(w).time - (double)si2->at(i).TimestampMaxLayer(1,false);
					if (timediff < 201 && timediff >= 191) TimingGammaIndex[w]++;
					TDiffSX3->Fill(timediff);
				}

				if (gamEn > 506 && gamEn < 514) gam511.push_back(gamEn);
				else gam511.push_back(0);


			}
			//***********************************END TIMING********************************************


			if (si2->at(i).telescopeID == 1100 && si2->at(i).MultLayer(0,false) == 1 && si2->at(i).MultLayer(1,false) == 1){

				//E1 is the normalized x (uncalibrated Energy) E1 value. They are normalized to the rstrip=0 histogram. E1 norm is the normalized E1 energy for each dE strip (i.e. dE strip 0, E strip 0, 1, 2, 3)
				E1norm = E1*normDA0[stripdE][rstrip]+normDA1[stripdE][rstrip];

				//dEcal is the calibrated dE energy. 
				dEcal = calDAdE[stripdE][0]*dE + calDAdE[stripdE][1];

				//E1cal is the calibrated E1 energy, assuming a linear calibration for the 3 highest E peaks. 
				E1cal = E1norm*calDAE1[stripdE][0]+calDAE1[stripdE][1];

				//Variables for the high and low triton PID gates for use in the below if statement are defined here.
				PIDGateHi = multhiDA[stripdE]*pow(E1,exphiDA[stripdE]);
				PIDGateLo = multloDA[stripdE]*pow(E1,exploDA[stripdE]);

				PIDGateHiCal = pow(E1cal,4)*CalPIDGateHi[stripdE][0] + pow(E1cal,3)*CalPIDGateHi[stripdE][1] + pow(E1cal,2)*CalPIDGateHi[stripdE][2] + E1cal*CalPIDGateHi[stripdE][3] + CalPIDGateHi[stripdE][4];
				PIDGateLoCal = pow(E1cal,4)*CalPIDGateLo[stripdE][0] + pow(E1cal,3)*CalPIDGateLo[stripdE][1] + pow(E1cal,2)*CalPIDGateLo[stripdE][2] + E1cal*CalPIDGateLo[stripdE][3] + CalPIDGateLo[stripdE][4];

				//Here we take only the events within the restricted gates. Now with even more restriction!
				if (dE < PIDGateHi && dE > PIDGateLo){
				if (dEcal < PIDGateHiCal && dEcal > PIDGateLoCal){

					tritongate++;

					SiTime.push_back((double)si2->at(i).time);

					//DA_PIDhists is the gated triton PIDs. 
					DA_PIDhists[stripdE]->Fill(E1,dE);
					//These 3 histograms are the energy vs strip number histograms.
					if (dE != 0) QQQDAdE->Fill(dE,stripdE);
					if (E1 != 0) QQQDAE1->Fill(E1,stripE1);
					if (E2 != 0) QQQDAE2->Fill(E2,stripE2);

					//The following histogram was to gate on the gamma lines and try to see what tritons were in coincidence. It didn't work because there were too few counts.
					//if (stripdE == 0 && (stripE1 == 0 || stripE1 == 1) && gamgate>0) QQQDAdE0->Fill(E1);

					//Now we only consider the events that have strip(E1)-strip(dE) between 0 and 4. 
					if (rstrip >= 0 && rstrip <= 4){
						
						//This vector puts the E1 energy spectra for each strip(dE) into individual histograms. They are stored in the DA_Triton_Spectra folder.
						DA_T_Spec[stripdE][rstrip]->Fill(E1);	
						
						//normDA0 and normDA1 are the m and b to the mx+b normalization, respectively. The normalization takes the E1 histograms and adds them to the rstrip=0 histogram for each strip(dE).
						if (normDA0[stripdE][rstrip] != 0 ){


							//QQQDAE1sum are the summed normalized E1 histograms. 
							QQQDAE1sum->Fill(E1norm, stripdE);
							

							//QQQDAE1cal are the calibrated DAE1 histograms.
							QQQDAE1cal->Fill(E1cal, stripdE);


							//QQQDAdEcal are the calibrated DAdE histograms.
							QQQDAdEcal->Fill(dEcal, stripdE);

							DA_PIDhists_Cal[stripdE]->Fill(E1cal,dEcal);

							Ecaltot = E1cal + dEcal;
					
							//DA_TotE stores the total energy histograms in the DA_Total_E folder.
							DA_TotE[stripdE]->Fill(Ecaltot);
							//QQQDATot stores the calibrated energy in a strip # vs calibrated energy histogram. 
							QQQDATot->Fill(Ecaltot, stripdE);
							
							if (stripdE < 20){
								//QQQDATot2 stores the calibrated energy BEFORE the Al blocker in strip # vs energy.
								angle = AandEloss[stripdE][0];

								Einit = pow(Ecaltot,3)*AandEloss[stripdE][2]+pow(Ecaltot,2)*AandEloss[stripdE][3]+Ecaltot*AandEloss[stripdE][4]+AandEloss[stripdE][5];

								QQQDATot2->Fill(Einit, stripdE);

								//Q-Value Spectrum, calculation of Q-value below. Assume 30MeV for beam energy. Equation 11.10 in Krane.
								Q = Einit*(1+m3H/m19Ne)-30*(1-m3He/m19Ne)-2*sqrt(m3He*m3H/pow(m19Ne,2)*Einit*30)*cos(angle);
								QAl = Einit*(1+m3H/m27Si)-30*(1-m3He/m27Si)-2*sqrt(m3He*m3H/pow(m27Si,2)*Einit*30)*cos(angle);

								Ex19Ne = (m3He + m19F - m3H - m19Ne)*utoMeV - Q;

								QQQDAQ->Fill(Q, stripdE);
								QQQDAQAl->Fill(QAl, stripdE);
								QQQDATotQ->Fill(Q);
					
								QQQDAEx->Fill(Ex19Ne, stripdE);
								QQQDAExTot->Fill(Ex19Ne);
									
								//Q-value gate
								if (Q < -7.2 && Q > -7.4) Qgate++;
/****/								if (Ex19Ne > 6.2 && Ex19Ne < 6.6) Exgate++;

								//Put the for loop for the range of Ex gates here
								for (int ex = 0; ex < 31; ex++){
									if (Ex19Ne > ExGateArrLow[ex] && Ex19Ne < ExGateArrHi[ex]) ExGates[ex]++;
								}//closes ex for loop

								//Q-value gate for the intensity histograms filled here.
//								for (int bin = 0; bin < 299; bin++){
//									if (Q < Qbins[bin+1] && Q > Qbins[bin]) Qgatearray[bin][1]++;
//								}//closes the Q-bin gate
								//Qgatevec.push_back(Q);

								//Loop through the Q-value gates determined from the gamma intensity histograms and fill the Q_gated_ histograms.
								for (int g = 0; g < 5; g++){
									if (Q > Qgatelo[g] && Q < Qgatehi[g]){
										Q_gated_E1[stripdE]->Fill(E1norm);
										Q_DA_dE_Gated[stripdE][g]->Fill(dE);
									}//closes if Qgate statement
								}//closes g for loop

							}//closes if strip(dE)<20 

							//The gates are set here. If the gate for each peak is greater than 0, the gamma histogram corresponding to that peak is filled later in the program.
							//There are 4 peaks that were gated on, and each peak has two bounds (hence the p<8). The for loop is incremented by 2 because the 2nd bound is used by doing p+1.
							for (int p = 0; p<8; p+=2){
		
								//if E1norm is less than the first bound and greater than the 2nd bound...
								if (E1norm < boundDA[stripdE][p] && E1norm > boundDA[stripdE][p+1]){
									//...increase the gate of that peak (0,1,2,3) by 1.
									pkgate[p/2]++;
	
									//DA_dE_Gated is the gated dE for each of the four peaks in E1. p/2 because there is 4 peaks
									DA_dE_Gated[stripdE][p/2]->Fill(dE);

								} //closes the gating if statement

							} //closes the gating for loop

						} //closes the if the normalization parameter !=0 if statement.

					} //closes the rstrip condition if statement

					

					if (timegate > 0) QQQDAExtotTiming->Fill(Ex19Ne);

				} //closes the restricted gates if statement
				} //closes resitricted gates if statement

			} //closes the DA if statement

	//********************************************DC Begin*******************************************************************//
			if (si2->at(i).telescopeID == 1102 && si2->at(i).MultLayer(0,false) == 1){

				stripdE = stripdE;
				stripE1 = stripE1;
				dE = dE;
				E1 = E1;

				E1norm = pow(E1,2)*normDC0[stripdE][rstrip]+E1*normDC1[stripdE][rstrip]+normDC2[stripdE][rstrip];

				PIDGateLo = pow(E1,4)*TGateDCLow[stripdE][0] + pow(E1,3)*TGateDCLow[stripdE][1] + pow(E1,2)*TGateDCLow[stripdE][2] + E1*TGateDCLow[stripdE][3] + TGateDCLow[stripdE][4];
				PIDGateHi = pow(E1,4)*TGateDCHi[stripdE][0] + pow(E1,3)*TGateDCHi[stripdE][1] + pow(E1,2)*TGateDCHi[stripdE][2] + E1*TGateDCHi[stripdE][3] + TGateDCHi[stripdE][4];


				if (stripdE < 22 && dE > PIDGateLo && dE < PIDGateHi){
					tritongate++;
					if (stripdE == stripE1-1) DC_PIDhists[stripdE]->Fill(E1,dE);
					if (dE != 0) QQQDCdE->Fill(dE,stripdE);
					if (E1 != 0) QQQDCE1->Fill(E1,stripE1);
				
					if (rstrip >= 0 && rstrip <= 4){
						 DC_T_Spec[stripdE][rstrip]->Fill(E1);

						if ((E1norm > 14600 && stripdE == 1) || (stripdE == 2 && E1norm > 16700) || (stripdE == 3 && E1norm > 14500) || (stripdE == 4 && E1norm > 14500) || (stripdE == 6 && E1norm > 12000)){
							QQQDCE1sum->Fill(E1norm,stripdE);

							if (stripdE == 6 && E1norm > 23800 && E1norm < 25000) DCgate++;

							Q = pow(E1norm,2)*DCtoQ[stripdE][0]+E1norm*DCtoQ[stripdE][1]+DCtoQ[stripdE][2];

							//Q-value gate for the intensity histograms filled here.
							for (int bin = 0; bin < 299; bin++){
								if (Q < Qbins[bin+1] && Q > Qbins[bin]) Qgatearray[bin][1]++;
							}//closes the Q-bin gate
						
							QQQDCQ->Fill(Q, stripdE);

							Ex19Ne = (m3He + m19F - m3H - m19Ne)*utoMeV - Q;

							QQQDCEx->Fill(Ex19Ne, stripdE);
							QQQDCExTot->Fill(Ex19Ne);

/*****/						if (Ex19Ne > 6.3 && Ex19Ne < 6.7) Exgate++;//DCgate++;

							//Put the for loop for the range of Ex gates here
							//Digital Excitation energy spectrum not great below 3.9 MeV...
							if (Ex19Ne > 3.9){
								for (int ex = 0; ex < 31; ex++){
									if (Ex19Ne > ExGateArrLow[ex] && Ex19Ne < ExGateArrHi[ex]) ExGates[ex]++;
								}//closes ex for loop
							}
						}

					} //closes rstrip if statement
				} //closes triton gate if statement

			} //closes DC if statement

	//********************************************DC End*******************************************************************//

			//Downstream barrel Si detectors for loop
			if (si2->at(i).telescopeID >= 2104 && si2->at(i).telescopeID <= 2110 && si2->at(i).telescopeID != 2106 && si2->at(i).MultLayer(0,false) == 1 && si2->at(i).MultLayer(1,false) == 1 && stripdE > -1 && stripE1 > -1){

				if (stripdE == 0 && stripE1 != 3) break;
				if ((stripdE == 1 || stripdE == 2) && (stripE1 != 3 && stripE1 != 2)) break;
				if ((stripdE == 3 || stripdE == 4) && (stripE1 != 2 && stripE1 != 1)) break;
				if (stripdE == 5 && stripE1 != 1) break;
				if ((stripdE == 6 || stripdE == 7) && stripE1 !=0) break;

				if (positionE1 >=60) sec = 3;
				if (positionE1 >=40 && positionE1 < 60) sec = 2;
				if (positionE1 >=20 && positionE1 < 40) sec = 1;
				if (positionE1 < 20) sec = 0;

				switch(si2->at(i).telescopeID) {
					case 2104: D4_PID[stripdE][stripE1][sec]->Fill(E1,dE);
						   break;
					case 2105: D5_PID[stripdE][stripE1][sec]->Fill(E1,dE);
						   break;
					case 2107: D7_PID[stripdE][stripE1][sec]->Fill(E1,dE);
						   break;
					case 2108: D8_PID[stripdE][stripE1][sec]->Fill(E1,dE);
						   break;
					case 2109: D9_PID[stripdE][stripE1][sec]->Fill(E1,dE);
						   break;
					case 2110: D10_PID[stripdE][stripE1][sec]->Fill(E1,dE);
						   break;
				}//closes PID Switch

			}

			//if (si2->at(i).telescopeID == 2105 && stripdE == 0 && stripE1 == 3 && positionE1 > 77 && dE > -14943*log(E1)+211302-500 && dE < -16012*log(E1)+227462+5000){
			if (si2->at(i).telescopeID == 2105 && stripdE == 0 && stripE1 == 3 && positionE1 > 60 && positionE1<63 && dE > 4.142e-18*pow(E1,4)-4.29676e-12*pow(E1,3)+1.8723e-6*pow(E1,2)-.454376*E1+7.31e4 && dE < (3.462e-19*pow(E1,4)-5.12e-13*pow(E1,3)+4.92e-7*pow(E1,2)-.2428*E1+6.53e4)){
				E1proj->Fill(E1);
				dEE1check->Fill(E1,dE);
			}

			double SX3Norm[4] = {1,1.021282496,1.027428233,1.05339854};

			double SX3E1Norm = E1*SX3Norm[stripE1];

			D5posZ->Fill(positionE1);
			D5posXY->Fill(sqrt(pow(si2->at(i).PosE1().X(),2)+pow(si2->at(i).PosE1().Y(),2)));

			//if (positionE1 > 80) positionE1=80;
			if (positionE1 < 0) positionE1=0;
			angle = -100;
			if (sqrt(pow(si2->at(i).PosE1().X(),2)+pow(si2->at(i).PosE1().Y(),2)) > 0) angle = atan(sqrt(pow(si2->at(i).PosE1().X(),2)+pow(si2->at(i).PosE1().Y(),2))/positionE1) * 180 / pi;

			//if (g238 > 0 || g275>0 || g1233>0 || g1297>0 || g2556>0){
				if (si2->at(i).telescopeID == 2105){// && positionE1>70 && positionE1 < 75){
					if (stripE1==0) SX3_0->Fill(E1);
					if (stripE1==1) SX3_1->Fill(E1);
					if (stripE1==2) SX3_2->Fill(E1);
					if (stripE1==3) SX3_3->Fill(E1);

					if (g238 > 0 || g275 > 0){
						D5_E3->Fill(SX3E1Norm);						
						D5vPos->Fill(SX3E1Norm,positionE1);
						D5vAng->Fill(SX3E1Norm,angle);
					}
					if (gtest1 > 0 || gtest2 > 0){
						D5_E4->Fill(SX3E1Norm);
						D5vPos2->Fill(SX3E1Norm,positionE1);
						D5vAng2->Fill(SX3E1Norm,angle);

					}
		
					if (g275 > 0) D5_E->Fill(SX3E1Norm);
					if (gtest2 > 0) D5_E2->Fill(SX3E1Norm);

					if (SX3E1Norm > 440000 && SX3E1Norm < 640000) SX3Gate++;
				}
			//}

		for (unsigned int f = 0; f < gam2->size();f++){
			gamEn = pow(gam2->at(f).en,2)*gamcalparams[gam2->at(f).num][0] + gam2->at(f).en*gamcalparams[gam2->at(f).num][1] + gamcalparams[gam2->at(f).num][2]; 

			if (gamEn != 0 && TimingGammaIndex[f]>0 && Ex19Ne >6.2 && Ex19Ne < 6.6) QQQDAvsGam->Fill(Ex19Ne,gamEn); 

		}
		}//closes silicon detectors loop



		for (unsigned int i = 0; i < gam2->size();i++){
		if (SX3Gate > 0 && gCRAP == 0){//g196 == 0 && g890 == 0 && g754 == 0 && g109 == 0){
			
			gamEn = pow(gam2->at(i).en,2)*gamcalparams[gam2->at(i).num][0] + gam2->at(i).en*gamcalparams[gam2->at(i).num][1] + gamcalparams[gam2->at(i).num][2]; 

			gam_ind->Fill(gamEn,gam2->at(i).num);

			

			//*******Gamma histograms filled below here are suppressed by the time gate with the Si detectors*******
			if (gamEn != 0 && TimingGammaIndex[i]>0){

				//for (unsigned int j = 0; j < gam2->size();j++){
				//	if (gamEn > 3000 && gam511[j] > 0 && TimingGammaIndex[j] > 0){
				//		gamEn += gam511[j];
						//std::cout << "woah!" << std::endl;
				//	}
				//}
				gam_tot->Fill(gamEn);
				//Fill the gated gammasphere histograms here:
				for (int z=0; z<4; z++){
					if (pkgate[z] > 0) gam_gated[z]->Fill(gamEn);

				}
				for (int ex = 0; ex < 31; ex++){
					if (ExGates[ex] > 0) ExGatedGams[ex]->Fill(gamEn);
				}

				if (Qgate > 0){
					gam_gated[4]->Fill(gamEn);
				}
				if (Exgate > 0){
					gam_gated[5]->Fill(gamEn);
					if (gamEn > 1825 && gamEn < 1830) gcount1++;
					if (gamEn > 1842 && gamEn < 1845) gcount2++;
				}

				if (DCgate > 0){
					gam_gated[6]->Fill(gamEn);
				}

				for (int q = 0; q < 300; q++){
					if (Qgatearray[q][1]>0){

						if (gamEn > 234 && gamEn < 239) int238->Fill(Qgatearray[q][0]);
						if (gamEn > 241 && gamEn < 246) back238->Fill(Qgatearray[q][0]);

						if (gamEn > 270 && gamEn < 276) int275->Fill(Qgatearray[q][0]);
						if (gamEn > 260 && gamEn < 266) back275->Fill(Qgatearray[q][0]);

						if (gamEn > 1224 && gamEn < 1240) int1232->Fill(Qgatearray[q][0]);
						if (gamEn > 1196 && gamEn < 1212) back1232->Fill(Qgatearray[q][0]);

						if (gamEn > 1835 && gamEn < 1847) int1840->Fill(Qgatearray[q][0]);
						if (gamEn > 1823 && gamEn < 1835) back1840->Fill(Qgatearray[q][0]);

						if (gamEn > 2526 && gamEn < 2576) int2556->Fill(Qgatearray[q][0]);
						if (gamEn > 2584 && gamEn < 2634) back2556->Fill(Qgatearray[q][0]);

						if (gamEn > 4138 && gamEn < 4148) int4140->Fill(Qgatearray[q][0]);
						if (gamEn > 4128 && gamEn < 4138) back4140->Fill(Qgatearray[q][0]);

						if (gamEn > 4294 && gamEn < 4376) int4362->Fill(Qgatearray[q][0]);
						if (gamEn > 4400 && gamEn < 4482) back4362->Fill(Qgatearray[q][0]);

					}
				}
					


				if (g238 > 0) gam238->Fill(gamEn);
				if (g275 > 0) gam275->Fill(gamEn);
				if (g1233 > 0) gam1233->Fill(gamEn);
				if (g1297 > 0) gam1297->Fill(gamEn);
				if (g1840 > 0) gam1840->Fill(gamEn);
				if (g2556 > 0 && g238 > 0) gam2556->Fill(gamEn);
			}
	
		}

		}

		for (int c=0; c<300; c++){ Qgatearray[c][1] = 0;}
		timegate = 0;
		SiTime.clear();
		TimingGammaIndex.clear();
	}



	
	
	std::cout << "Run " << runnum << " will now be closed." << std::endl; 
	gam2->clear();
	si2->clear();
	data->Close();


}

//********************************************************************************************************//


void MakeMyHists(){
	gcount1 = 0;	
	gcount2 = 0;
	int stripnum = 32;
	//the hist file is opened here for writing
	hist = TFile::Open("SX3Tests.root","RECREATE");
	std::ofstream logFile("GoddessAnalysis.log");

	//directories for the histograms are created. 
	Particle_IDs = hist->mkdir("Particle_IDs");
		QQQ5_DA_PID = hist->mkdir("Particle_IDs/DA_PID");
		QQQ5_DA_PID_Cal = hist->mkdir("Particle_IDs/DA_PID_Cal");
		QQQ5_DC_PID = hist->mkdir("Particle_IDs/DC_PID");
		D4_FB_PIDs = hist->mkdir("Particle_IDs/D4_FB_PIDs");
		D5_FB_PIDs = hist->mkdir("Particle_IDs/D5_FB_PIDs");
		D7_FB_PIDs = hist->mkdir("Particle_IDs/D7_FB_PIDs");
		D8_FB_PIDs = hist->mkdir("Particle_IDs/D8_FB_PIDs");
		D9_FB_PIDs = hist->mkdir("Particle_IDs/D9_FB_PIDs");
		D10_FB_PIDs = hist->mkdir("Particle_IDs/D10_FB_PIDs");
	Triton_Spectra = hist->mkdir("Triton_Spectra");
		DA_Triton_Spectra = hist->mkdir("Triton_Spectra/DA_Triton_Spectra");
		DC_Triton_Spectra = hist->mkdir("Triton_Spectra/DC_Triton_Spectra");
		D5_Triton_Spectra = hist->mkdir("Triton_Spectra/D5_Triton_Spectra");
	Calibration_Hists = hist->mkdir("Calibration_Hists");
		QQQ5_DA_dE_gated = hist->mkdir("Calibration_Hists/DA_dE_gated");
		Q_Gated_QQQ_DA_E1 = hist->mkdir("Calibration_Hists/Q_Gated_QQQ_DA_E1");
		Q_Gated_QQQ_DA_dE = hist->mkdir("Calibration_Hists/Q_Gated_QQQ_DA_dE");
	Gamma_Hists = hist->mkdir("Gamma_Hists");
		Gammasphere_Hists = hist->mkdir("Gamma_Hists/Gammasphere_Histograms");
		Gamma_Gated_Hists = hist->mkdir("Gamma_Hists/Gamma_Gated_Histograms");
		Gamma_Intensity_Hists = hist->mkdir("Gamma_Hists/Gamma_Intensity_Histograms");
		Excitation_Energy_Gated = hist->mkdir("Gamma_Hists/Excitation_Energy_Gated");
	QQQ5_DA_TotalE = hist->mkdir("DA_Total_E");
	QQQ5_DC_TotalE = hist->mkdir("DC_Total_E");
	Strip_vs_E_Hists = hist->mkdir("Strip_vs_E_Hists");
	Timing_Spectra = hist->mkdir("Timing_Spectra");

	E1proj = new TH1D("E1proj","BB10 0 SX3 3 D5 sec 2", 1250, 0, 900000);
	dEE1check = new TH2D("dEE1check","check",1250,0,900000,1250,0,300000);

	SX3_0 = new TH1D("SX3_0","SX3 2105 strip 0 gated",5000,0,1000000);
	SX3_1 = new TH1D("SX3_1","SX3 2105 strip 1 gated",5000,0,1000000);
	SX3_2 = new TH1D("SX3_2","SX3 2105 strip 2 gated",5000,0,1000000);
	SX3_3 = new TH1D("SX3_3","SX3 2105 strip 3 gated",5000,0,1000000);

	D5_E = new TH1D("D5_E","SX3 2105",500,0,1000000);
	D5_E2 = new TH1D("D5_E2","SX3 2105",500,0,1000000);
	D5_E3 = new TH1D("D5_E3","SX3 2105",500,0,1000000);
	D5_E4 = new TH1D("D5_E4","SX3 2105",500,0,1000000);

	D5vPos = new TH2D("D5vPos","SX3 Pos vs Energy",500,0,1000000,16,0,80);
	D5vPos2 = new TH2D("D5vPos2","SX3 Pos vs Energy",500,0,1000000,16,0,80);
	D5vAng = new TH2D("D5vAng","SX3 Angle vs Energy",500,0,1000000,30,30,90);
	D5vAng2 = new TH2D("D5vAng2","SX3 Angle vs Energy",500,0,1000000,30,30,90);

	D5posZ = new TH1D("D5posZ","D5 Z Position",150,-40,110);
	D5posXY = new TH1D("D5posXY","D5 XY Position",150,-40,110);

	//QQQ energy histograms are created here.
	Strip_vs_E_Hists->cd();
	QQQDAdE = new TH2D("QQQDAdE","QQQ5 DA dE",4096,0,4096,32,0,32);
	QQQDAE1 = new TH2D("QQQDAE1","QQQ5 DA E1",4096,0,4096,32,0,32);
	QQQDAE2 = new TH2D("QQQDAE2","QQQ5 DA E2",4096,0,4096,32,0,32);

	QQQDCdE = new TH2D("QQQDCdE","QQQ5 DC dE",1250,0,50000,32,0,32);
	QQQDCE1 = new TH2D("QQQDCE1","QQQ5 DC E1",1250,0,50000,32,0,32);

	QQQDAE1sum = new TH2D("QQQDAE1sum","QQQ5 DA E1 Summed Histograms",1024,0,1024,32,0,32);
	QQQDAE1cal = new TH2D("QQQDAE1cal","QQQ5 DA E1 Calibrated Histograms",512,0,32,32,0,32);
	QQQDAdEcal = new TH2D("QQQDAdEcal","QQQ5 DA dE Calibrated Histograms",512,0,32,32,0,32);

	QQQDCE1sum = new TH2D("QQQDCE1sum","QQQ5 DC E1 Summed Histograms",1250,0,50000,10,0,10);
	
	QQQDAvsGam = new TH2D("QQQDAvsGam","Gamma Energy vs Ex",2048,0,15,8000,0,8000);

	QQQ5_DA_TotalE->cd();
	QQQDATot = new TH2D("QQQDATot","QQQ5 DA Total Energy",2048,0,32,32,0,32);
	QQQDATot2 = new TH2D("QQQDATot2","QQQ5 DA Total Energy Before Degradation",2048,15,32,19,0,19);
	QQQDAQ = new TH2D("QQQDAQ","Strip Number vs Reaction Q-Value Spectrum",2048,-15,0,19,0,19);
	QQQDAQAl = new TH2D("QQQDAQAl","Strip Number vs Reaction Q-Value Spectrum 27Al(3He,t)",2048,-15,0,19,0,19);
	QQQDATotQ = new TH1D("QQQDATotQ","QQQ5 DA Q Summed Q-Value Spectrum",2048,-15,0);
	QQQDAEx = new TH2D("QQQDAEx","Strip Number vs Excitation Energy",2048,0,15,19,0,19);
	QQQDAExTot = new TH1D("QQQDAExTot","Excitation Energy of 19Ne",2048,0,15);

	//Test histograms for timing are created here.
	QQQDAExtotTiming = new TH1D("QQQDAExtotTiming","QQQ5 DA Total E Timing Gated",2048,0,15);

	std::string gambase = "gam_pk";

	QQQ5_DC_TotalE->cd();
	QQQDCQ = new TH2D("QQQDCQ","Strip Number vs Reaction Q-Value Spectrum",2048,-15,0,19,0,19);
	QQQDCEx = new TH2D("QQQDCEx","Strip Number vs Excitation Energy",2048,0,15,19,0,19);
	QQQDCExTot = new TH1D("QQQDCExTot","Excitation Energy of 19Ne",2048,0,15);

	hist->cd("Gamma_Hists/Gammasphere_Histograms");
	gam_ind = new TH2D("gam_ind","Individual Gammasphere Detectors",10000,0,10000,111,0,111);
	gam_tot = new TH1D("gam_tot","Summed Gammasphere Spectrum",8000,0,8000);


	gam_ind->GetXaxis()->SetTitle("Channel Number");
	gam_ind->GetYaxis()->SetTitle("Gammasphere Detector Number");
	gam_ind->GetXaxis()->CenterTitle();
	gam_ind->GetYaxis()->CenterTitle();

	gam_tot->GetXaxis()->SetTitle("Energy (keV)");
	gam_tot->GetYaxis()->SetTitle("Counts");
	gam_tot->GetXaxis()->CenterTitle();
	gam_tot->GetYaxis()->CenterTitle();


	for (int i=0; i<7; i++){
		string namegam = gambase + std::to_string(i);
		TH1D *h = new TH1D(TString(namegam),"Gated Gammasphere Spectrum Triton Peak " + TString(std::to_string(i)),8000,0,8000);

		gam_gated.push_back(h);

		gam_gated.back()->GetXaxis()->SetTitle("Energy (keV)");
		gam_gated.back()->GetYaxis()->SetTitle("Counts");
		gam_gated.back()->GetXaxis()->CenterTitle();
		gam_gated.back()->GetYaxis()->CenterTitle();
	}
	QQQDAdE0 = new TH1D("QQQDAdE0","Summed QQQ DA dE Strip 0 gated on 6125 keV gamma peak",4096,0,4096);

	//Total summed energy from both dE and E detectors
	//QQQDAtot = new TH1D("QQQDAtot","Total Energy Strip 0",8000,0,16);
	
	//base histogram names for histogram vectors
	std::string PIDbaseDA = "pid_DA_";
	std::string PIDbaseDC = "pid_DC_";
	std::string TSpecBasedE = "triton_dE_";
	std::string TSpecBaseE = "_E_";
	std::string dEgatebase = "dE_";
	std::string baseDAtot = "E_tot_";
	std::string QE1base = "Q_gated_E1_";
	std::string QdEbase = "Q_gated_dE_";
		
	for (int i=0; i<stripnum; i++){
		hist->cd("Particle_IDs/DA_PID");
		//Creates the summed DA PID plots (with coincidences of PstripE-PstripdE <=4)
		string nameDA = PIDbaseDA + std::to_string(i);
		TH2D *h0 = new TH2D(TString(nameDA),"QQQ5 DA dE vs E1 Strip " + TString(std::to_string(i)),1024,0,1024,4096,0,4096);					

		DA_PIDhists.push_back(h0);

		DA_PIDhists.back()->GetXaxis()->SetTitle("Channel Number in E1");
		DA_PIDhists.back()->GetYaxis()->SetTitle("Channel Number in dE");
		DA_PIDhists.back()->GetXaxis()->CenterTitle();
		DA_PIDhists.back()->GetYaxis()->CenterTitle();

		hist->cd("Particle_IDs/DA_PID_Cal");
		TH2D *g0 = new TH2D(TString(nameDA)+"_cal","QQQ5 DA dE vs E1 Strip " + TString(std::to_string(i)),1024,0,32,500,0,6);					

		DA_PIDhists_Cal.push_back(g0);

		DA_PIDhists_Cal.back()->GetXaxis()->SetTitle("E1 Energy (MeV)");
		DA_PIDhists_Cal.back()->GetYaxis()->SetTitle("dE Energy (MeV)");
		DA_PIDhists_Cal.back()->GetXaxis()->CenterTitle();
		DA_PIDhists_Cal.back()->GetYaxis()->CenterTitle();

		hist->cd("Particle_IDs/DC_PID");
		//Creates the summed DC PID plots (with coincidences of PstripE-PstripdE <=4)
		string nameDC = PIDbaseDC + std::to_string(i);
		TH2D *h1 = new TH2D(TString(nameDC),"QQQ5 DC dE vs E1 Strip " + TString(std::to_string(i)),1250,0,50000,1250,0,50000);

		DC_PIDhists.push_back(h1);

		DC_PIDhists.back()->GetXaxis()->SetTitle("Channel Number in E1");
		DC_PIDhists.back()->GetYaxis()->SetTitle("Channel number in dE");
		DC_PIDhists.back()->GetXaxis()->CenterTitle();
		DC_PIDhists.back()->GetYaxis()->CenterTitle();

		hist->cd("Triton_Spectra/DA_Triton_Spectra");
		//Creates the individual trition energy spectrum for each DA PstripdE and PstripE combination. Therefore, the vector created will be of dimension 32x5.
		for (int j=0; j<5; j++){
			int k = i+j;

			string TnameDA = TSpecBasedE + std::to_string(i) + TSpecBaseE + std::to_string(k);
			TH1D *h2 = new TH1D(TString(TnameDA),"Triton Energy dE Strip " + TString(std::to_string(i)) + " E Strip " + TString(std::to_string(k)),4096,0,4096);
			sT_Spec.push_back(h2);

			sT_Spec.back()->GetXaxis()->SetTitle("Channel Number");
			sT_Spec.back()->GetYaxis()->SetTitle("Counts");
			sT_Spec.back()->GetXaxis()->CenterTitle();
			sT_Spec.back()->GetYaxis()->CenterTitle();
		}

		DA_T_Spec.push_back(sT_Spec);
		sT_Spec.clear();

		hist->cd("Triton_Spectra/DC_Triton_Spectra");
		//Creates the individual trition energy spectrum for each DC PstripdE and PstripE combination. Therefore, the vector created will be of dimension 32x5.
		for (int j=0; j<5; j++){
			int k = i+j;

			string TnameDC = TSpecBasedE + std::to_string(i) + TSpecBaseE + std::to_string(k);
			TH1D *h3 = new TH1D(TString(TnameDC),"Triton Energy dE Strip " + TString(std::to_string(i)) + " E Strip " + TString(std::to_string(k)), 5000,0,50000);
			sT_Spec.push_back(h3);

			sT_Spec.back()->GetXaxis()->SetTitle("Channel Number");
			sT_Spec.back()->GetYaxis()->SetTitle("Counts");
			sT_Spec.back()->GetXaxis()->CenterTitle();
			sT_Spec.back()->GetYaxis()->CenterTitle();

		}
		
		DC_T_Spec.push_back(sT_Spec);
		sT_Spec.clear();


		hist->cd("Calibration_Hists/DA_dE_gated");

		for (int j=0; j<4; j++){
			
			string dEgatename = dEgatebase + std::to_string(i) + std::to_string(j);
			TH1D *h4 = new TH1D(TString(dEgatename),"DA dE " + TString(std::to_string(i)) + " gated on peak " + TString(std::to_string(j)),4096,0,4096);
			dE_gated.push_back(h4);

		}

		DA_dE_Gated.push_back(dE_gated);
		dE_gated.clear();

		QQQ5_DA_TotalE->cd();

		string nametotDA = baseDAtot + std::to_string(i);
		TH1D *h5 = new TH1D(TString(nametotDA),"QQQ5 DA Total Energy Strip " + TString(std::to_string(i)),512,0,32);					

		DA_TotE.push_back(h5);
		DA_TotE.back()->GetXaxis()->SetTitle("Energy (MeV)");
		DA_TotE.back()->GetYaxis()->SetTitle("Counts");
		DA_TotE.back()->GetXaxis()->CenterTitle();
		DA_TotE.back()->GetYaxis()->CenterTitle();

		hist->cd("Calibration_Hists/Q_Gated_QQQ_DA_E1");
		string QE1gated = QE1base + std::to_string(i);
		TH1D *h6 = new TH1D(TString(QE1gated),"Q Gated QQQ DA E1 Strip " + TString(std::to_string(i)),4096,0,4096);
		Q_gated_E1.push_back(h6);


		hist->cd("Calibration_Hists/Q_Gated_QQQ_DA_dE");
		for (int j=0; j<5; j++){
			string QdEgated = QdEbase + std::to_string(i) + std::to_string(j);	
			TH1D *h7 = new TH1D(TString(QdEgated),"Q Gated QQQ DA dE Strip " + TString(std::to_string(i)),4096,0,4096);
			Q_gated_dE.push_back(h7);
		}

		Q_DA_dE_Gated.push_back(Q_gated_dE);
		Q_gated_dE.clear();
		
	}

	//Initialization of the BB10 vs SX3 histograms is here:
	string FBbase;
	string FBname;

	for (int k = 0; k < 6; k++){

		switch (k){
			case 0: hist->cd("Particle_IDs/D4_FB_PIDs");
				FBbase = "D4_";
				break;
			case 1: hist->cd("Particle_IDs/D5_FB_PIDs");
				FBbase = "D5_";
				break;
			case 2: hist->cd("Particle_IDs/D7_FB_PIDs");
				FBbase = "D7_";
				break;
			case 3: hist->cd("Particle_IDs/D8_FB_PIDs");
				FBbase = "D8_";
				break;
			case 4: hist->cd("Particle_IDs/D9_FB_PIDs");
				FBbase = "D9_";
				break;
			case 5: hist->cd("Particle_IDs/D10_FB_PIDs");
				FBbase = "D10_";
				break;
		}//closes switch statement

		//Pos sector: -30 to -10 sector 0, -10 to 10 sector 1, 10 to 30 sector 2, 30 to 50 sector 3

		//these vectors should be identified by vec[BB10 strip][SX3 strip][pos sector]


		for (int p = 0; p < 8; p++){

			for (int n = 0; n < 4; n++){


				for (int s = 0; s < 4; s++){

					if (p == 0 && n != 3) break;
					if ((p == 1 || p == 2) && (n != 3 && n != 2)) break;
					if ((p == 3 || p == 4) && (n != 2 && n != 1)) break;
					if (p == 5 && n != 1) break;
					if ((p == 6 || p == 7) && n !=0) break;

					FBname = FBbase + std::to_string(p) + "_" + std::to_string(n) + "_sec" + std::to_string(s);

					TH2D *h8 = new TH2D(TString(FBname),"BB10 Strip " + TString(std::to_string(p)) + " SX3 Strip " + TString(std::to_string(n)) + " Position Sector " + TString(std::to_string(s)),1250,0,900000,1250,0,300000);

					PIDCont.push_back(h8);

				} //closes position sector loop

				PIDCont2.push_back(PIDCont);
				PIDCont.clear();

			} //closes SX3 strip number loop
	
			switch (k){
				case 0: D4_PID.push_back(PIDCont2);
					break;
				case 1: D5_PID.push_back(PIDCont2);
					break;
				case 2: D7_PID.push_back(PIDCont2);
					break;
				case 3: D8_PID.push_back(PIDCont2);
					break;
				case 4: D9_PID.push_back(PIDCont2);
					break;
				case 5: D10_PID.push_back(PIDCont2);
					break;
				}//closes switch statement
			
			PIDCont2.clear();

		}//closes BB10 strip number loop

	}//closes outer switch statement loop

	Timing_Spectra->cd();

	TDiffDA = new TH1D("TDiffDA","Timing Difference Between Gammasphere and QQQ5 DA",1000,-500,500);
	TDiffDC = new TH1D("TDiffDC","Timing Difference Between Gammasphere and QQQ5 DC",1000,-500,500);
	TDiffSX3 = new TH1D("TDiffSX3","Timing Difference Between Gammasphere and SX3",1000,-500,500);


	gam_DA = new TH2D("gam_DA","Timing Difference Between Gammasphere and QQQ5 DA",2000,-1000,1000,32,0,32);
	gam_DC = new TH2D("gam_DC","Timing Difference Between Gammasphere and QQQ5 DC",2000,-1000,1000,32,0,32);

	TimeHist = new TH1D("TimeHist","Timing Difference for different QQQ5 Peaks",1000,-500,500);

	gamTime = new TH2D("gamTime","Gamma Energy vs Timing Difference", 1000,-500,500,4000,0,8000);

	gam511vtime = new TH1D("gam511vtime","Gamma 511 intensity vs timing",1000,-500,500); 
	gam511vtimeBACK = new TH1D("gam511vtimeBACK","Gamma 511 intensity vs timing",1000,-500,500);

	gam238vtime = new TH1D("gam238vtime","Gamma 238 intensity vs timing",1000,-500,500); 
	gam238vtimeBACK = new TH1D("gam238vtimeBACK","Gamma 238 intensity vs timing",1000,-500,500);

	gam1233vtime = new TH1D("gam1233vtime","Gamma 1233 intensity vs timing",1000,-500,500); 
	gam1233vtimeBACK = new TH1D("gam1233vtimeBACK","Gamma 1233 intensity vs timing",1000,-500,500);

	gam1633vtime = new TH1D("gam1633vtime","Gamma 1633 intensity vs timing",1000,-500,500); 
	gam1633vtimeBACK = new TH1D("gam1633vtimeBACK","Gamma 1633 intensity vs timing",1000,-500,500);

	hist->cd("Gamma_Hists/Gamma_Gated_Histograms");
	gam238 = new TH1D("gam238","Gammas Gated on 237 keV Peak",8000,0,8000);
	gam275 = new TH1D("gam275","Gammas Gated on 272 keV Peak",8000,0,8000);
	gam1233 = new TH1D("gam1233","Gammas Gated on 1233 keV Peak",8000,0,8000);
	gam1297 = new TH1D("gam1297","Gammas Gated on 1297 keV Peak",8000,0,8000);
	gam1840 = new TH1D("gam1840","Gammas Gated on 1840 keV Peak",8000,0,8000);
	gam2556 = new TH1D("gam2556","Gammas Gated on 2556 keV Peak",8000,0,8000);

	hist->cd("Gamma_Hists/Gamma_Intensity_Histograms");

	int238 = new TH1D("int238","Intensity of 238 keV Gamma vs Q-Value",300,-15,0);
	back238 = new TH1D("back238","Background of 238 keV Gamma vs Q-Value",300,-15,0);

	int275 = new TH1D("int275","Intensity of 275 keV Gamma vs Q-Value",300,-15,0);
	back275 = new TH1D("back275","Background of 275 keV Gamma vs Q-Value",300,-15,0);

	int1232 = new TH1D("int1232","Intensity of 1232 keV Gamma vs Q-Value",300,-15,0);
	back1232 = new TH1D("back1232","Background of 1232 keV Gamma vs Q-Value",300,-15,0);

	int1840 = new TH1D("int1840","Intensity of 1840 keV Gamma vs Q-Value",300,-15,0);
	back1840 = new TH1D("back1840","Background of 1840 keV Gamma vs Q-Value",300,-15,0);

	int2556 = new TH1D("int2556","Intensity of 2556 keV Gamma vs Q-Value",300,-15,0);
	back2556 = new TH1D("back2556","Background of 2556 keV Gamma vs Q-Value",300,-15,0);

	int4140 = new TH1D("int4140","Intensity of 4140 keV Gamma vs Q-Value",300,-15,0);
	back4140 = new TH1D("back4140","Background of 4140 keV Gamma vs Q-Value",300,-15,0);

	int4362 = new TH1D("int4362","Intensity of 4362 keV Gamma vs Q-Value",300,-15,0);
	back4362 = new TH1D("back4362","Background of 4362 keV Gamma vs Q-Value",300,-15,0);

	hist->cd("Gamma_Hists/Excitation_Energy_Gated");
	double gatenum=0;

	for (int i=0; i<31; i++){
		ExGateArrLow[i] = gatenum;
		gatenum += 0.3;
		ExGateArrHi[i] = ExGateArrLow[i]+0.5;

		string ExNameBase = "Ex_" + std::to_string(ExGateArrLow[i]) + "_" + std::to_string(ExGateArrHi[i]);
		
		TH1D *h9 = new TH1D(TString(ExNameBase),"Gated Gammasphere Spectrum Ex " + TString(std::to_string(ExGateArrLow[i])) + " to " + TString(std::to_string(ExGateArrHi[i])),8000,0,8000);

		ExGatedGams.push_back(h9);

	}

	hist->cd("Triton_Spectra/D5_Triton_Spectra");

//	for (int p = 0; p < 80; p+5){
//		int p2 = p+5;
//		string D5NameBase = "D5_" + std::to_string(p) + "_" + std::to_string(p2);
//		TH1D *h10 = new TH1D(TString(D5NameBase),"SX3 D5 Positions " + TString(std::to_string(p)) + " to " + TString(std::to_string(p2)),5000,0,1000000);
//
//		D5Spec.push_back(h10);
//	}


	//The files for the normalization parameters and gates are opened here.
	ifstream inFile0("textfiles/AnglesAndELoss2.txt");	
	ifstream inFile1("textfiles/E1normmultDA.txt");
	ifstream inFile2("textfiles/E1normaddDA.txt");
	ifstream inFile3("textfiles/gates.txt");
	ifstream inFile4("textfiles/dEECal2.txt");
	//ifstream inFile4("E1cal.txt");
	//ifstream inFile5("dEcal.txt");
	ifstream inFile6("textfiles/GamCalParamsQuadREDO.txt");
//	ifstream inFile6("textfiles/GamCalLinWHighREDOcompat.txt");
//	ifstream inFile7("angleassign.txt");
//	ifstream inFile8("energyloss.txt");
	ifstream inFile9("textfiles/Qbins.txt");
	ifstream inFile10("textfiles/QQQDCTGates.txt");
	ifstream inFile11("textfiles/CalGatesLo.txt");
	ifstream inFile12("textfiles/CalGatesHi.txt");

	ifstream inFile13("textfiles/E1normmult2DC.txt");
	ifstream inFile14("textfiles/E1normmult1DC.txt");
	ifstream inFile15("textfiles/E1normaddDC.txt");
	ifstream inFile16("textfiles/DCtoQ.txt");

	for (int i = 0; i<20; i++){

		inFile0 >> AandEloss[i][0] >> AandEloss[i][1] >> AandEloss[i][2] >> AandEloss[i][3] >> AandEloss[i][4] >> AandEloss[i][5];

	} 

	//build the arrays for the gates and normalization parameters:

	for (int i=0; i<32; i++){

		inFile4 >> calDAE1[i][0] >> calDAE1[i][1] >> calDAdE[i][0] >> calDAdE[i][1];

		inFile16 >> DCtoQ[i][0] >> DCtoQ[i][1] >> DCtoQ[i][2];

//		inFile4 >> calDAE1[i][0] >> calDAE1[i][1];
//		inFile5 >> calDAdE[i][0] >> calDAdE[i][1];
		
		for (int k = 0; k<8; k++){

			inFile3 >> boundDA[i][k];

		}

		for (int j=0; j<5; j++){

			inFile1 >> normDA0[i][j];
			inFile2 >> normDA1[i][j];

			inFile13 >> normDC0[i][j];
			inFile14 >> normDC1[i][j];
			inFile15 >> normDC2[i][j];
			
		}

//		if (i<20) inFile7 >> stripangle[i][0] >> stripangle[i][1];

//		if (i<12) inFile8 >> energyloss[i][0] >> energyloss[i][1] >> energyloss[i][2] >> energyloss[i][3] >> energyloss[i][4] >> energyloss[i][5];

		inFile11 >> CalPIDGateLo[i][0] >> CalPIDGateLo[i][1] >> CalPIDGateLo[i][2] >> CalPIDGateLo[i][3] >> CalPIDGateLo[i][4];
		inFile12 >> CalPIDGateHi[i][0] >> CalPIDGateHi[i][1] >> CalPIDGateHi[i][2] >> CalPIDGateHi[i][3] >> CalPIDGateHi[i][4];


	}

	for (int i=0; i<111; i++){

		inFile6 >> gamcalparams[i][0] >> gamcalparams[i][1] >> gamcalparams[i][2];

	}

	for (int i=0; i<300; i++){
		
		inFile9 >> Qbins[i] >> buff >> Qgatearray[i][0] >> Qgatearray[i][1];

		if (i < 44){
			if (i < 22) inFile10 >> TGateDCLow[i][0] >> TGateDCLow[i][1] >> TGateDCLow[i][2] >> TGateDCLow[i][3] >> TGateDCLow[i][4];
			if (i > 21) inFile10 >> TGateDCHi[i-22][0] >> TGateDCHi[i-22][1] >> TGateDCHi[i-22][2] >> TGateDCHi[i-22][3] >> TGateDCHi[i-22][4];
		}

	}

//	for (int i=0; i<22; i++){
//		std::cout << TGateDCHi[i][0] << " " << TGateDCHi[i][1] << " " << TGateDCHi[i][2] << " " << TGateDCHi[i][3] << " " << TGateDCHi[i][4] << std::endl;
//
//	}

	//close the files here
	inFile0.close();
	inFile1.close();
	inFile2.close();
	inFile3.close();
	inFile4.close();
//	inFile5.close();
	inFile6.close();
//	inFile7.close();
//	inFile8.close();
	inFile9.close();
	inFile10.close();



	int numruns = 0;
	for (int run = 404; run < 495; run++){
		//Only some runs actually have data and will be made into histograms using this code.
		if ((run > 400 && run < 409 && run != 405) || (run > 409 && run < 424) || (run > 431 && run < 435) || (run > 436 && run < 447) || (run > 448 && run < 455) || (run > 455 && run < 458) || (run > 459 && run < 465) || (run > 468 && run < 484) || (run > 484 && run < 495)){
			histfill(run,logFile);
			numruns++;
		}
	}
	
	int238->Add(back238,-1);
	int275->Add(back275,-1);
	int1232->Add(back1232,-1);
	int1840->Add(back1840,-1);
	int2556->Add(back2556,-1);
	int4140->Add(back4140,-1);
	int4362->Add(back4362,-1);

	gam511vtime->Add(gam511vtimeBACK,-1);
	gam238vtime->Add(gam238vtimeBACK,-1);
	gam1233vtime->Add(gam1233vtimeBACK,-1);

//	SX3_0->Add(SX3_2,-1);
//	SX3_1->Add(SX3_3,-1);

	D5_E->Add(D5_E2,-1);
	D5_E3->Add(D5_E4,-1);	

	D5vPos->Add(D5vPos2,-1);
	D5vAng->Add(D5vAng2,-1);	

	std::cout << std::endl << "The number of runs analyzed was: " << numruns << std::endl;
	std::cout << "Writing data to TotalData.root" << std::endl;
	hist->Write();
	hist->Close();

	logFile.close();

	std::cout << gcount1 << " * " << gcount2 << std::endl;

}



//Old code that works and has been taken out of the program is dumped here at the end.







	//Creates the summed DA PID plots (with coincidences of PstripE-PstripdE <=4)
/*	QQQ5_DA_PID->cd();
	for (int i=0; i<stripnum; i++){
		string name = PIDbaseDA + std::to_string(i);
		TH2D *h = new TH2D(TString(name),"QQQ5 DA dE vs E1 Strip " + TString(std::to_string(i)),4096,0,4096,4096,0,4096);

		name->GetXaxis()->SetTitle("Channel Number");
		name->GetYaxis()->SetTitle("Counts");
		name->GetXaxis()->CenterTitle();
		name->GetYaxis()->CenterTitle();						

		DA_PIDhists.push_back(h);
	}

*/
/*
	//Creates the summed DC PID plots (with coincidences of PstripE-PstripdE <=4)
	QQQ5_DC_PID->cd();
	for (int i=0; i<stripnum; i++){
		string name = PIDbaseDC + std::to_string(i);
		TH2D *h = new TH2D(TString(name),"QQQ5 DC dE vs E1 Strip " + TString(std::to_string(i)),5000,0,50000,5000,0,50000);
		DC_PIDhists.push_back(h);
	}

*/
/*
	//Creates the individual trition energy spectrum for each DA PstripdE and PstripE combination. Therefore, the vector created will be of dimension 32x5.
	QQQ5_DA_Tritons->cd();
	for (int i=0; i<stripnum; i++){
		for (int j=0; j<5; j++){
			int k = i+j;
			string name = TSpecBasedE + std::to_string(i) + TSpecBaseE + std::to_string(k);
				TH1D *h = new TH1D(TString(name),"Triton Energy dE Strip " + TString(std::to_string(i)) + " E Strip " + TString(std::to_string(k)), 4096,0,4096);
				sT_Spec.push_back(h);
		}
		DA_T_Spec.push_back(sT_Spec);
		sT_Spec.clear();
	}

	//Creates the individual trition energy spectrum for each DC PstripdE and PstripE combination. Therefore, the vector created will be of dimension 32x5.
	QQQ5_DC_Tritons->cd();
	for (int i=0; i<stripnum; i++){
		for (int j=0; j<5; j++){
			int k = i+j;
			string name = TSpecBasedE + std::to_string(i) + TSpecBaseE + std::to_string(k);
				TH1D *h = new TH1D(TString(name),"Triton Energy dE Strip " + TString(std::to_string(i)) + " E Strip " + TString(std::to_string(k)), 4096,0,4096);
				sT_Spec.push_back(h);
		}
		DC_T_Spec.push_back(sT_Spec);
		sT_Spec.clear();
	}






				//if (stripE1 ==1 && stripdE==0 && j==0) QQQDAtot->Fill(dE*0.000529+1.23 + E1*.0053+7.8547);

				//if (dE < 911 && stripdE == 0 && (stripE1 == 0 || stripE1 == 1)){
				//	QQQDAdE0->Fill(E1);
					
				//}

				//if (E1 > 586 && E1 < 612 && stripdE == 0 && (stripE1 == 0 || stripE1 == 1)) gate++;







	//strip number angle assignments.
//	double stripangle[20][2];
	
	//cubic energy loss parameters through the Al blocker. In the file, Column 1: Angle in Deg, Column 2: Angle in Rad, Column 3,4,5,6 are the parameters Ax^3+Bx^2+Cx+D in that order.
//	double energyloss[12][6];



*/

//gamma data for loop for timing histograms 
/*
					for (unsigned int w = 0; w < gam2->size();w++){

						gamEn = pow(gam2->at(w).en,2)*gamcalparams[gam2->at(w).num][0] + gam2->at(w).en*gamcalparams[gam2->at(w).num][1] + gamcalparams[gam2->at(w).num][2]; 

						timediff = (double)gam2->at(w).time - (double)si2->at(i).TimestampMaxLayer(0,false);
				
						if (gamEn > 505 && gamEn < 515) gam511vtime->Fill(timediff);
						if (gamEn > 490 && gamEn < 500) gam511vtimeBACK->Fill(timediff);

						if (gamEn > 232 && gamEn < 239) gam238vtime->Fill(timediff);
						if (gamEn > 225 && gamEn < 232) gam238vtimeBACK->Fill(timediff);

						if (gamEn > 1223 && gamEn < 1241) gam1233vtime->Fill(timediff);
						if (gamEn > 1205 && gamEn < 1223) gam1233vtimeBACK->Fill(timediff);

						if (gamEn > 1628 && gamEn < 1639) gam1633vtime->Fill(timediff);
						if (gamEn > 1675 && gamEn < 1686) gam1633vtimeBACK->Fill(timediff);

						gam_DA->Fill(timediff,stripdE);
						if ((timediff < 216 && timediff > 176) || (timediff < 430 && timediff > 400)) timegate++;
						if (Ex19Ne > 5.15) TimeHist->Fill(timediff);

						gamTime->Fill(timediff,gamEn);

						if ( (73 < timediff && timediff < 104) || (190 < timediff && timediff < 217) || (416 < timediff && timediff < 440)) QQQDAvsGam->Fill(Ex19Ne,gamEn); 

					}
*/

