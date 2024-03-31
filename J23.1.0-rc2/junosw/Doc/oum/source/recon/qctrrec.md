# Reconstruction using QCtrRecAlg

**QCtrRecAlg** is developed for fast reconstruction for OEC.
**QCtrRecAlg** implements the Simplified Charge based Maximum Likelihood Estimation vertex and energy reconstruction method (Guihong)(**SQMLE**),
the Peak Time Fitter vertex reconstruction method (Ziyuan)  + Total Charge with Uniformity Correction energy reconstruction method (Guihong)(**ptf**), 
and the Hit-Weighted Charge Center vertex reconstruction method (Iwan) + Total Charge with Uniformity Correction energy reconstruction method (Guihong)(**imb**). 

**QCtrRecAlg** realizes a simple energy steering strategy. Firstly, QCtrRecAlg checks the Total Chrage energy NQE by calling **QSumTool** (Guihong). Then, QCtrRecAlg will call one of the vertex and energy reconstruction methods when NQE < 100 MeV. Otherwise, vertex will not be reconstructed and the energy will not be optimized.

This doc briefly introduces the principle of these methods and provides instructions for users.


## Method Principle
SQMLE balances the reconstruction speed and accuracy. Comparing to QMLE, SQMLE ignores the theta dependence of the nPE map and the differences of the charge spectrum of PMTs.
For more details, `JUNO-doc-5405` introduced the principle of "ptf", `JUNO-doc-9739` introduced the principle of "imb", `JUNO-doc-9758` compared the performances of these methods.

* **Code location** : `\$JUNOTOP/junosw/Reconstruction/QCtrRecAlg/src`


## User Interface
The vertex and energy reconstruction algorithms are loaded and configured by `tut_calib2rec.py (JUNOEventRecModule.py)`.
* **Location** : `\$TUTORIALROOT/share`

The default method of offline is "energy-point". The default method of OEC is "qctr". User can specify `--method qctr` to use QCtrRecAlg. The default method of QCtrRecAlg is "sqmle". 

The recommended command is 
```
python tut_calib2rec.py --evtmax -1  --input calib.root --output rec.root --user-output user-rec.root --method qctr

```



## Output
QCtrRecAlg results are saved as RecVertex in CdVertexRecEvt in the EDM file rec.root. The saved variables are
```
              Vertex : m_x, m_y, m_z //position. Unit:mm
              Energy : m_energy      //Best estimation of deposit energy. Unit:MeV
        
```



