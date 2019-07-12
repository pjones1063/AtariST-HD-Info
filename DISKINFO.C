#include "DISKINFO.H"
#include <aes.h>		/* get the AES prototypes and definitions */
#include <vdi.h>		/* get the VDI prototypes and definitions */
#include <osbind.h>

/*******************************************************/
/*   DISKINFO.C   LATTICE/C                            */
/*                                                     */
/*   Simple program to demo Atari 16 bit RSC files     */
/*                                                     */
/*******************************************************/

#define FMD_START 0
#define FMD_GROW 1
#define FMD_SHRINK 2
#define FMD_FINISH 3
#define R_TREE 0
#define FINGER 3

/* define some constant arrays from the RSC*/
const int free[] = {FRE1, FRE2, FRE3, FRE4, FRE5, FRE6, FRE7, FRE8, FRE9, FREA, FREB, FREC, FRED};
const int prec[] = {PRC1, PRC2, PRC3, PRC4, PRC5, PRC6, PRC7, PRC8, PRC9, PRCA, PRCB, PRCC, PRCD};
const int cpdd[] = {CPD1, CPD2, CPD3, CPD4, CPD5, CPD6, CPD7, CPD8, CPD9, CPDA, CPDB, CPDC, CPDD};
const int bpsc[] = {BPS1, BPS2, BPS3, BPS4, BPS5, BPS6, BPS7, BPS8, BPS9, BPSA, BPSB, BPSC, BPSD};
const int spcc[] = {SPC1, SPC2, SPC3, SPC4, SPC5, SPC6, SPC7, SPC8, SPC9, SPCA, SPCB, SPCC, SPCD};

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
  unsigned long free,
				cpd,
				bps,
				spc;
};

/* disk info array */
struct diskinfos {
    char    free[10],		 
            cpd[10],
	    prc[10],
	    bps[10],
	    spc[10];
};


/* Global variables */
int handle, dum, drive_map;
int x, y, w, h, n_x, n_y; 
OBJECT * dialog; 


/* set the text object value */
int setText(int i, char * value) 
{
   TEDINFO *ob_tedinfo;
   ob_tedinfo = (TEDINFO *) dialog[i].ob_spec;	
   ob_tedinfo -> te_ptext = value;
   return 1;
}

/* display the results  */
int setDiskInfo(struct diskinfos * dis, int disk) {
   setText(free[disk], dis->free);
   setText(cpdd[disk], dis->cpd);
   setText(prec[disk], dis->prc);
   setText(spcc[disk], dis->spc);
   setText(bpsc[disk], dis->bps);
   return 1;
}	


/* get info for one hard disk */
struct diskinfos getDiskInfo(int disk) 
{	
	struct diskinfo di;
        struct diskinfos dis;
	char temp_str[10];
	long percent;
	if (drive_map & (1 << (disk-1)) ) 
    { 
	 getdfree(disk, &di); 
  	 sprintf (dis.free,"%ld", (di.free * di.bps * di.spc) / 1024);	
	 sprintf (dis.cpd,"%ld", (di.cpd * di.bps * di.spc) / 1024);	
    
         percent = (di.cpd * di.bps * di.spc) - (di.free * di.bps * di.spc);
	 percent = (percent*100) / (di.cpd * di.bps * di.spc); 			 
         sprintf (dis.prc,"%ld", percent );	
	
	 sprintf (dis.bps,"%ld", di.bps);
	 sprintf (dis.spc,"%ld", di.spc);		
    } 
    else 
    {
     strcpy (dis.free, "  ");	
     strcpy (dis.cpd,  "  ");
     strcpy (dis.prc,  "  ");
     strcpy (dis.bps,  "  ");	
     strcpy (dis.spc,  "  ");	
	}		  
    return dis;
}


/*main program */
int main(void)
{	
	struct diskinfos dinfos[16];	
	short handle, junk;		
        int ch, i, rez, d;
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
	  
	  for(d=0; d < 13; d++) {       /* get info on 13 hd disks */
	      dinfos[d] = getDiskInfo(d+3);  
	      setDiskInfo(&dinfos[d], d);
	   }
	  
	  graf_mouse (FINGER,&junk);    /* finish up the display */
          form_dial (FMD_START , 0, 0, 10, 10, x, y, w, h) ;
	  form_dial (FMD_GROW , 0, 0, 10, 10, x, y, w, h) ;
          objc_draw (dialog, 0, 10, x, y, w, h) ;
	  
      do {							/* loop intil exit click*/
          ch = form_do (dialog, DG1) ; 
	  } while (ch != BT1);
	  
	  form_dial (FMD_SHRINK , 0, 0, 10, 10, x, y, w, h) ;
          form_dial (FMD_FINISH , 0, 0, 10, 10, x, y, w, h) ;
    }
	
        v_clsvwk(handle);				/* close workstation */
	appl_exit();					/* shutdown AES */
	return 0;
}
