# Reconstruction using OMILREC

**OMILREC** (Optical Model Independent Likelihood REConstruction) implements the Charge based Maximum Likelihood Estimation (**QMLE**) and 
the Charge and Time combined Maximum Likelihood Estimation (**QTMLE**) vertex and energy reconstruction methods.

This doc briefly introduces the principle of these methods and the structure of the algorithm, and provides instructions for users.


## Method Principle
The deposited energy of the event is converted into photoelectrons. The nPE and hit time information of PMTs preserve
the vertex and energy information of the event. QMLE/QTMLE are data driven reconstruction methods for point like event. 
The event vertex and energy are fitted by maximizing the likelihood function. The nPE maps and time pdfs are constructed 
from ACU+CLS calibration data. More details can be found in `JUNO-doc-6191` and `JUNO-doc-6381`.


## Algorithme Structure
The algorithm diagram is shown in the figure below. User can read the source code for more details.

![Fig. Diagram of `QTMLE`](figs/QTMLE.png)

* **Code location** : `offline/Reconstruction/OMILREC/src`
* **Maps/pdfs location** : `data/Reconstruction/OMILREC/RecMap`


## User Interface
The vertex and energy reconstruction algorithms are loaded and configured by `tut_calib2rec.py (JUNOEventRecModule.py)`.
* **Location** : `offline/Examples/Tutorial/share`

The default algorithm is "RecTimeLikeAlg (point)". User can specify `--method energy-point` to use OMILREC.

The recommended command is 
```
python tut_calib2rec.py --evtmax -1  --input calib.root --output rec.root --user-output user-rec.root --method energy-point $RecOpt

# Run QTMLE to reconstruct vertex and energy
RecOpt="--enableLTSPEs --enableTimeInfo  --enableUseEkMap"

# Run QMLE to reconstruct vertex and energy
RecOpt="--enableLTSPEs --enableUseEkMap" 

# Run QMLE to reconstruct energy using vertex from RecTimeLikeAlg
RecOpt="--enableLTSPEs --enableUseEkMap --enableUseTLHVertex"

enableLTSPEs: enable MCP-PMT’s long tail SPES (doc-5727-v1...)
enableTimeInfo: enable time information for QTMLE (doc-6191-v2...)
enableUseEkMap: enable using 68Ge+Laser nPE map (doc-6237-v2...)
```





## Output
Variables in user-rec.root
```
         Final vertex : recx

          QMLE vertex : recQx
          TMLE vertex : recTx
         QTMLE vertex : recQTx
             QTMLE t0 : m_FadcEvtT 

RecTimeLikeAlg vertex : timeRecx

          QMLE energy : m_QEn
         QTMLE energy : m_QTEn
 Energy in p.e. units : m_PESum
  Total charge energy : m_NQE 
         Total charge : m_NPE

           Time usage : m_Qtime, m_Ttime, m_QTtime
     Likelihood value : m_QL, m_TL, m_QTL
```


Variables in rec.root
```
              Vertex : m_x
              Energy : m_energy (or m_eprec)
Energy in p.e. units : m_PEsum
     t0 (m_FadcEvtT) : m_t0    
    QTMLE likelihood : m_chisq
        Energy error : m_energyQuality
        Radius error : m_positionQuality
        
```



