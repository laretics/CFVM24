#include "mtfLock.h"

#ifndef _CFVM_LOCK_
#define _CFVM_LOCK_

#define CUSTOM_BLOCK_DEF circuit* colcE01[3] = { &c02, &c04, &m1 };\
	uint8_t collE01[3] = { 0, 0, 0 };\
	signal* colsE01[2] = { &A2,&E2 };\
	signal::tSignalOrder colsoE01[2] = { signal::viaLibre,signal::avisoParada };\
	circuit* colcE02[3] = { &c02, &c04, &m2 };\
	uint8_t collE02[3] = { 0, 1, 0 };\
	signal* colsE02[2] = { &A2,&E2 };\
	signal::tSignalOrder colsoE02[2] = { signal::precaucion,signal::avisoParada };\
	circuit* colcE03[3] = { &c01, &c03, &m3 };\
	uint8_t collE03[3] = { 0, 1, 0 };\
	signal* colsE03[2] = { &A1,&E1 };\
	signal::tSignalOrder colsoE03[2] = { signal::precaucion,signal::avisoParada };\
	circuit* colcE04[3] = { &c01, &c03, &m4 };\
	uint8_t collE04[3] = { 0, 2, 0 };\
	signal* colsE04[2] = { &A1,&E1 };\
	signal::tSignalOrder colsoE04[2] = { signal::precaucion,signal::avisoParada };\
	circuit* colcS01[1] = { &c06 };\
	uint8_t collS01[1] = { 0 };\
	signal* colsS01[1] = { &S1 };\
	signal::tSignalOrder colsoS01[1] = { signal::viaLibre };\
	circuit* colcS02[1] = { &c06 };\
	uint8_t collS02[1] = { 1 };\
	signal* colsS02[1] = { &S2 };\
	signal::tSignalOrder colsoS02[1] = { signal::viaLibre };\
	circuit* colcS03[1] = { &c08 };\
	uint8_t collS03[1] = { 1 };\
	signal* colsS03[1] = { &S3 };\
	signal::tSignalOrder colsoS03[1] = { signal::viaLibre };\
	circuit* colcS04[1] = { &c08 };\
	uint8_t collS04[1] = { 0 };\
	signal* colsS04[1] = { &S3 };\
	signal::tSignalOrder colsoS04[1] = { signal::viaLibre };\
	circuit* colcP01[4] = { &c02, &c04, &m1, &c06 };\
	uint8_t collP01[4] = { 0, 0, 0,0 };\
	signal* colsP01[3] = { &A2,&E2,&S1 };\
	signal::tSignalOrder colsoP01[3] = { signal::viaLibre,signal::viaLibre,signal::viaLibre };\
	circuit* colcP02[4] = { &c02, &c04, &m2, &c06 };\
	uint8_t collP02[4] = { 0, 1, 0,1 };\
	signal* colsP02[3] = { &A2,&E2,&S2 };\
	signal::tSignalOrder colsoP02[3] = { signal::precaucion,signal::precaucion,signal::viaLibre };\
	circuit* colcP03[4] = { &c01, &c03, &m3, &c08 };\
	uint8_t collP03[4] = { 0, 1, 0,1 };\
	signal* colsP03[3] = { &A1,&E1,&S3 };\
	signal::tSignalOrder colsoP03[3] = { signal::precaucion,signal::precaucion,signal::viaLibre };\
	circuit* colcP04[4] = { &c01, &c03, &m4, &c08 };\
	uint8_t collP04[4] = { 0, 2, 0,0 };\
	signal* colsP04[3] = { &A1,&E1,&S4 };\
	signal::tSignalOrder colsoP04[3] = { signal::precaucion,signal::viaLibre,signal::viaLibre };\
	circuit* colcP05[4] = { &c01, &c03, &c05, &c07 };\
	uint8_t collP05[4] = { 0, 0, 0,0 };\
	signal* colsP05[3] = { &A1,&E1,&I1 };\
	signal::tSignalOrder colsoP05[3] = { signal::viaLibre,signal::viaLibre,signal::viaLibre };\
\
	itinerary itinE01, itinE02, itinE03, itinE04\
	, itinS01, itinS02, itinS03, itinS04\
	, itinP01, itinP02, itinP03, itinP04, itinP05;\
	itinerary* _colItinerary[13] = { &itinE01,&itinE02,&itinE03,&itinE04,\
				&itinS01,&itinS02,&itinS03,&itinS04,\
				&itinP01,&itinP02,&itinP03,&itinP04,&itinP05 };

class cfvmLock : public mtfLock
{
public:
	cfvmLock(Elegoo_TFTLCD* tft);
	void initialize_structure(); //Inicia la estructura
	void create_routes(); //Enlaza los paths.
private:
	circuitLayout c01_0;
	circuitLayout* c01_lay[1] = { &c01_0 };

	circuitLayout c02_0;
	circuitLayout* c02_lay[1] = { &c02_0 };

	circuitLayout c03_0;
	circuitLayout c03_1;
	circuitLayout c03_2;
	circuitLayout* c03_lay[3] = { &c03_0,&c03_1,&c03_2 };

	circuitLayout c04_0;
	circuitLayout c04_1;
	circuitLayout* c04_lay[2] = { &c04_0,&c04_1 };

	circuitLayout c06_0;
	circuitLayout c06_1;
	circuitLayout* c06_lay[2] = { &c06_0,&c06_1 };

	circuitLayout c07_0;
	circuitLayout* c07_lay[1] = { &c07_0 };

	circuitLayout c08_0;
	circuitLayout c08_1;
	circuitLayout* c08_lay[2] = { &c08_0,&c08_1 };

	circuitLayout m4_0;
	circuitLayout* m4_lay[1] = { &m4_0 };

	circuitLayout m3_0;
	circuitLayout* m3_lay[1] = { &m3_0 };

	circuitLayout m2_0;
	circuitLayout* m2_lay[1] = { &m2_0 };

	circuitLayout m1_0;
	circuitLayout* m1_lay[1] = { &m1_0 };

	circuitLayout c05_0;
	circuitLayout* c05_lay[1] = { &c05_0 };

	circuit c01, c02, c03, c04, c06, c07, c08, m4, m3, m2, m1, c05;
	int16_t _rawData[266] = {
		1,0,40,0,1,50,150,0,1,0,
		40,0,1,50,150,0,215,77,305,137,
		0,65,10,110,60,1,109,128,60,1,
		139,220,60,0,155,50,170,35,1,169,
		220,35,1,0,38,0,1,49,220,0,
		1,69,220,0,0,155,50,170,35,1,
		169,220,35,1,0,38,0,1,49,56,
		0,0,55,0,110,60,1,109,128,60,
		1,139,220,60,1,69,220,0,1,159,
		220,60,1,0,38,0,1,49,56,0,
		0,55,0,110,60,1,109,128,60,1,
		139,146,60,0,145,60,170,35,1,169,
		220,35,305,187,0,155,-10,170,-25,1,
		169,220,-25,1,0,128,0,1,139,220,
		0,1,159,220,0,1,0,128,0,1,
		139,146,0,0,145,0,170,-25,1,169,
		220,-25,705,187,1,0,71,-25,0,70,
		-25,83,-10,1,0,98,0,1,109,200,
		0,1,0,75,0,1,109,200,0,1,
		0,71,-25,0,70,-25,90,0,1,89,
		98,0,1,0,200,0,705,112,1,0,
		71,25,0,70,25,83,10,1,0,98,
		0,1,109,200,0,1,0,75,0,1,
		109,200,0,1,0,71,25,0,70,25,
		90,0,1,89,98,0,1,0,200,0,
		1,0,200,0,1,0,200,0,1,0,
		200,0,1,0,200,0
	};
	circuit* _colCircuit[12] = { &c01,&c02,&c03,&c04,&c06,&c07,&c08,&m4,&m3,&m2,&m1,&c05 };
	signal A1, A2, E1, E2, S1, S2, S3, S4, I1;
	signal* _colSignal[9] = { &A1,&A2,&E1,&E2,&S1,&S2,&S3,&S4,&I1 };
	platform ptf0, ptf1, ptf2, ptf3;
	platform* _colPlatform[4] = { &ptf0,&ptf1,&ptf2,&ptf3 };

	/// <summary>
	/// Esto no viene en la generaci�n autom�tica... hay que importarlo a mano.
	/// </summary>
	CUSTOM_BLOCK_DEF
};

#endif // !_CFVM_LOCK_
