# Reconstruct single muon using LsqMuonRecTool
`CdWpMuonRecTool` mainly use the first hit time (FHT) of 20-inch PMTs in CD to reconstruct the track of single cosmic muon event. The rec. tool is located in the `junosw/Reconstruction/RecMuonAlg/LsqMuonRecTool/`

## Algorithm Principle
The algorithm describe a single muon track with 4 parameters, the thetas & phis of the initial position on the CD and the direction.
- Obtain the initial fitting parameters by the ealiest hit position of PMT and the charge weight center
- The FHT of PMTs can be predicted by the fitting parameters.
- With the predicted FHT and detected FHT, the chi2 can be constructed
- Minimize the chi2ndf by the method, Minuit2

## How to use
```bash
python $TUTORIALROOT/share/tut_calib2rec.py --input calibPath --output recPath --method track
```

## RecCdMuonAlg
- `RecCdMuonAlg` is created to obtain the information of PMTs in CD.
- The info. obtained is stored into a PMT table, the structure of the table is
	* `pos`: the position of the PMT center
	* `q`: the charge of the PMT
	* `fht`: the first hit time of the PMT
	* `res`: the resolution of the PMT
	* `used`: if the PMT is used
	* `type`: the type of the PMT (20-inch or 3-inch)
