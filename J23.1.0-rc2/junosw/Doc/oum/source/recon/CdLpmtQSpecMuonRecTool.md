# Reconstruct muon using CdLpmtQSpecMuonRecTool
`CdLpmtQSpecMuonRecTool` mainly use the charge collected by 20-inch PMTs in CD to reconstruct the tracks of cosmic muon event. The rec. tool is located in the `junosw/Reconstruction/RecMuonAlg/CdLpmtQSpecMuonRecTool/`

## Algorithm Principle
The tool reconstruct tracks with only the charge collected by 20-inch PMTs in CD
- Project the charge information into a 2D distribution
- Find the charge clusters by TSpectrum method in ROOT
- Select the clusters by some cut
- Improve the cluster position reconstruction results with a limited charge-weight calculation
- Get tracks by choosing the best clusters combinations

## How to use
```bash
python $TUTORIALROOT/share/tut_calib2rec.py --input calibPath --output recPath --method lpmtQ-track
```
