# Reconstruction using RecTimeLikeAlg
`RecTimeLikeAlg` mainly use time information to reconstruct point-like event time and vertex for IBD. The event energy can also be reconstructed.

## Algorithm Principle
- Using the emission time profile of the scintillation photons, and taking into account the PMT effects (TTS and Dark noise), the event time and vertex can be reconstructed. 
- 1PE PDF is constructed using $e^{+}$ Mont Carlo samples.
- nPE PDFs are constructed using the so-called first photoelectron timing techniques.
- Charge Center and Peak Time Fitter algorithms are used to provide accurate initial values.
- More details can be found in JUNO-doc-5405 (1D PDF) and JUNO-doc-6563 (3D PDF).

## Code Flow
- The figure shows the code flow of `RecTimeLikeAlg`.
- `IDEAL` means no PMT effect, while `ELEC` means with electronic simulation. 
- More details can be found in : `$RECTIMELIKEALGROOT/src/RecTimeLikeAlg.cc`.
![Fig. Diagram of `rectimelike`](figs/rectimelike.png)


## Usage
### Reconstruct with 1D PDF
- Advantages : fast (~0.5s), low memory consumption
- Disadvantages : large bias in detector border region (~10cm)
```bash
$ python ${TUTORIALROOT}/share/tut_calib2rec.py --evtmax -1 --input $input --output $output --user-output $user_output $RecOpt
# without PMT effect : detsim -> det2calib -> calib2rec
RecOpt='--SwitchAlg 1 --elec no' 
# with electronic simulation : detsim -> det2elec -> elec2calib -> calib2rec 
RecOpt='--SwitchAlg 1 --elec yes' 
# with electronic simulation and correct the bias in detector border region 
RecOpt='--SwitchAlg 1 --elec yes --VtxCorr 1' 
```

### Reconstruct with 3D PDF
- Advantages : fast (~1s), small bias 
- Disadvantages : relatively large memory consumption
```bash
$ python ${TUTORIALROOT}/share/tut_calib2rec.py --evtmax -1 --input $input --output $output --user-output $user_output $RecOpt
# without PMT effect : detsim -> det2calib -> calib2rec
RecOpt='--SwitchAlg 2 --elec no' 
# with electronic simulation : detsim -> det2elec -> elec2calib -> calib2rec 
RecOpt='--SwitchAlg 2 --elec yes' 
```

## Output
* EDM root file
	* `m_x, m_y, m_z` : event vertex (mm)
	* `m_px` : event start time t0 (ns)
	* `m_py` : vertex likelihood
	* `m_pz` : energy likelihood
	* `m_energy` : deposited energy (MeV)
	* `m_eprec` : energy with non-uniformity and non-linearity correction (MeV)

* User root file
	* `rec_x, rec_y, rec_z, rec_r` : event vertex (mm)
	* `rec_t` : event start time t0 (ns)
	* `likelihood` : vertex likelihood
	* `recE_likelihood` : energy likelihood
	* `rec_n` : deposited energy (MeV)
	* `rec_E` : energy with non-uniformity and non-linearity correction (MeV)
