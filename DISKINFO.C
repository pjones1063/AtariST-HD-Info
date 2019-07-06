#include "DISKINFO.H"
#include <aes.h>		/* get the AES prototypes and definitions */
#include <vdi.h>		/* get the VDI prototypes and definitions */
#include <osbind.h>

/*******************************************************/
/*                                                     */
/*   DISKINFO.C   LATTICE/C                            */
/*                                                     */
/*                                                     */
/*   Display disk stats                                */
/*                                                     */
/*                                                     */
/*                                                     */
/*******************************************************/

#define FMD_START 0
#define FMD_GROW 1
#define FMD_SHRINK 2
#define FMD_FINISH 3
#define R_TREE 0
#define FINGER 3


/* v_opnvwk input array */
int work_in[11],
	work_out[57],
	pxyarray[10],
	contrl[12],
	intin[128],
	ptsin[128],
	intout[128],
	ptsout[128];

/* disk info array */
struct diskinfo {
  unsigned long  free,
				  cpd,
				  bps,
				  spc;
};

/* disk info array */
struct diskinfos {
  char  free[10],		 
         cpd[10],
		 prc[10],
         bps[10],
         spc[10];
};


/* Global variables */
int handle, dum, drive_map;
int x, y, w, h, n_x, n_y; 
OBJECT * dialog; 

int setText(int i, char * value) 
{
   TEDINFO *ob_tedinfo;
   ob_tedinfo = (TEDINFO *) dialog[i].ob_spec;	
   ob_tedinfo -> te_ptext = value;
   return 1;
}

struct diskinfos getDiskInfo(int disk) 
{	
	struct diskinfo di;
    struct diskinfos dis;
	char temp_str[10];
	long percent;
	if (drive_map & (1 << (disk-1)) ) 
    { 
	 getdfree(disk, &di); 
  	 sprintf (temp_str,"%ld", (di.free * di.bps * di.spc) / 1024);
     strcpy (dis.free, temp_str);	
	
	 sprintf (temp_str,"%ld", (di.cpd * di.bps * di.spc) / 1024);	
     strcpy (dis.cpd, temp_str);
    
     percent = (di.cpd * di.bps * di.spc) - (di.free * di.bps * di.spc);
	 percent = (percent*100) / (di.cpd * di.bps * di.spc); 		
	 
     sprintf (temp_str,"%ld", percent );	
     strcpy (dis.prc, temp_str);
	
	 sprintf (temp_str,"%ld", di.bps);
     strcpy (dis.bps, temp_str);	
	 sprintf (temp_str,"%ld", di.spc);
     strcpy (dis.spc, temp_str);		
    } 
    else 
    {
     strcpy (dis.free, "");	
     strcpy (dis.cpd,  "");
     strcpy (dis.prc,  "");
     strcpy (dis.bps,  "");	
     strcpy (dis.spc,  "");	
	}
    return dis;
}


int main(void)
{	
	struct diskinfos dinfos[16];	
	short handle, junk;		
	long error;
    int choice, i, rez;
	char temp_str[20];
	
	appl_init();		/* start AES */
	handle=graf_handle(&junk, &junk, &junk, &junk);	/* find AES handle */
	for (i=0; i<10; work_in[i++] = 1);
    work_in[10] = 2;
	v_opnvwk(work_in, &handle, work_out);			/* open workstation */
    rez = Getrez();	
	if(rez == 0)
	{	
	  form_alert (1,"[1][Med/High Rez only][I am done]");
	}	
	else if (!rsrc_load ("DISKINFO.RSC")) 
	{ 
      form_alert (1,"[1][DISKINFO.RSC is missing][All done]");
	}
	else
	{
      rsrc_gaddr(R_TREE, DG1, &dialog) ; 
      form_center (dialog, &x, &y,  &w , &h) ;
	  drive_map = setdisk (0); 
	  
	  dinfos[1] = getDiskInfo(3);  	  
	  setText(FRE1, dinfos[1].free);
	  setText(CPD1, dinfos[1].cpd);
	  setText(PRC1, dinfos[1].prc);
	  setText(SPC1, dinfos[1].spc);
	  setText(BPS1, dinfos[1].bps);

	  dinfos[2] = getDiskInfo(4);  	  
	  setText(FRE2, dinfos[2].free);
	  setText(CPD2, dinfos[2].cpd);
	  setText(PRC2, dinfos[2].prc);
	  setText(SPC2, dinfos[2].spc);
	  setText(BPS2, dinfos[2].bps);

	  dinfos[3] = getDiskInfo(5);  	  
	  setText(FRE3, dinfos[3].free);
	  setText(CPD3, dinfos[3].cpd);
	  setText(PRC3, dinfos[3].prc);
	  setText(SPC3, dinfos[3].spc);
	  setText(BPS3, dinfos[3].bps);

	  dinfos[4] = getDiskInfo(6);  	  
	  setText(FRE4, dinfos[4].free);
	  setText(CPD4, dinfos[4].cpd);
	  setText(PRC4, dinfos[4].prc);
	  setText(SPC4, dinfos[4].spc);
	  setText(BPS4, dinfos[4].bps);

	  dinfos[5] = getDiskInfo(7);  	  
	  setText(FRE5, dinfos[5].free);
	  setText(CPD5, dinfos[5].cpd);
	  setText(PRC5, dinfos[5].prc);
	  setText(SPC5, dinfos[5].spc);
	  setText(BPS5, dinfos[5].bps);

	  dinfos[6] = getDiskInfo(8);  	  
	  setText(FRE6, dinfos[6].free);
	  setText(CPD6, dinfos[6].cpd);
	  setText(PRC6, dinfos[6].prc);
	  setText(SPC6, dinfos[6].spc);
	  setText(BPS6, dinfos[6].bps);

	  dinfos[7] = getDiskInfo(9);  	  
	  setText(FRE7, dinfos[7].free);
	  setText(CPD7, dinfos[7].cpd);
	  setText(PRC7, dinfos[7].prc);
	  setText(SPC7, dinfos[7].spc);
	  setText(BPS7, dinfos[7].bps);

	  dinfos[8] = getDiskInfo(10);  	  
	  setText(FRE8, dinfos[8].free);
	  setText(CPD8, dinfos[8].cpd);
	  setText(PRC8, dinfos[8].prc);
	  setText(SPC8, dinfos[8].spc);
	  setText(BPS8, dinfos[8].bps);

	  dinfos[9] = getDiskInfo(12);  	  
	  setText(FRE9, dinfos[10].free);
	  setText(CPD9, dinfos[10].cpd);
	  setText(PRC9, dinfos[10].prc);
	  setText(SPC9, dinfos[10].spc);
	  setText(BPS9, dinfos[10].bps);

	  dinfos[10] = getDiskInfo(13);  	  
	  setText(FREA, dinfos[10].free);
	  setText(CPDA, dinfos[10].cpd);
	  setText(PRCA, dinfos[10].prc);
	  setText(SPCA, dinfos[10].spc);
	  setText(BPSA, dinfos[10].bps);

  	  dinfos[11] = getDiskInfo(14);  	  
	  setText(FREB, dinfos[11].free);
	  setText(CPDB, dinfos[11].cpd);
	  setText(PRCB, dinfos[11].prc);
	  setText(SPCB, dinfos[11].spc);
	  setText(BPSB, dinfos[11].bps);

  	  dinfos[12] = getDiskInfo(15);  	  
	  setText(FREC, dinfos[12].free);
	  setText(CPDC, dinfos[12].cpd);
	  setText(PRCC, dinfos[12].prc);
	  setText(SPCC, dinfos[12].spc);
	  setText(BPSC, dinfos[12].bps);
 	  
	  dinfos[13] = getDiskInfo(16);  	  
	  setText(FRED, dinfos[13].free);
	  setText(CPDD, dinfos[13].cpd);
	  setText(PRCD, dinfos[13].prc);
	  setText(SPCD, dinfos[13].spc);
	  setText(BPSD, dinfos[13].bps);

//	  dinfos[14] = getDiskInfo(17);  	  
//	  setText(FREE, dinfos[14].free);
//	  setText(CPDE, dinfos[14].cpd);
//	  setText(PRCE, dinfos[14].prc);
//	  setText(SPCE, dinfos[14].spc);
//	  setText(BPSE, dinfos[14].bps);
	  
//	  dinfos[15] = getDiskInfo(18);  	  
//	  setText(FREF, dinfos[15].free);
//	  setText(CPDF, dinfos[15].cpd);
//	  setText(PRCF, dinfos[15].prc);
//	  setText(SPCF, dinfos[15].spc);
//	  setText(BPSF, dinfos[14].bps);
	  
	  graf_mouse (FINGER,&junk);
      form_dial (FMD_START , 0, 0, 10, 10, x, y, w, h) ;
	  form_dial (FMD_GROW , 0, 0, 10, 10, x, y, w, h) ;
	  	   
      objc_draw (dialog, 0, 10, x, y, w, h) ;
      do {  
	  
       choice = form_do (dialog, DG1) ; 
		
	  } while (choice != BT1);
	  
	  form_dial (FMD_SHRINK , 0, 0, 10, 10, x, y, w, h) ;
      form_dial (FMD_FINISH , 0, 0, 10, 10, x, y, w, h) ;
    }
	
    v_clsvwk(handle);				/* close workstation */
	appl_exit();					/* shutdown AES */
	return 0;
}
