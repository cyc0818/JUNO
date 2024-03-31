# Understanding PMT optical model
## PMT optical model
PMT is quite important for our experiments. When a photon hit the photo-cathode of PMT, it will convert to photo-electron. Then this photo-electron will fly to anode in the electric field. The first part uses Quantum Efficiency (QE) to represent this conversion efficiency. The latter part uses Collection Efficiency (CE) to represent when a photo-electron is created, the efficiency it is collected by PMT. The final Detection Efficiency (DE) is:
$$ DE = QE \times CE $$

The processes are quite complicated. To describe PMT simulation more precisely, a data-driven PMT optical model is created. Both QE and CE are measured from several experiments. At the same time, the processes of optical photons are still simulated by Geant4.
$$ QE = QE(\lambda) $$
$$ CE = CE(\theta) $$
QE is a function of wavelength of optical photon, while CE is a function of position or latitude of PMT surface. 

## Implementation
In current detector simulation, an optical surface is used to represent photo-cathode. Please note, we don't create any volume whose material is photo-cathode in this optical model. 

When an optical photon hit this optical surface, Geant4 will invoke corresponding boundary process. First, we set the **reflectivity** of this optical surface to zero, no photons will be reflected. Then if we associate **QE** with this surface, Geant4 will sample it is hit or not. If it is hit, this optical photon will deposit its energy. Otherwise the photon will stop without any energy deposited. This allows us to decide a hit is generated or not at sensitive detector.

When in sensitive detector, we could get hit's position. So we could get corresponding collection efficiency at this position. By a simple sampling, we could decide the photo-electron is collected or not.

![Fig. PMT simulation work flow](figs/PMT-QE-CE.png)

## PMT geometry construction

In this PMT optical model, the key is how to construct a surface to represent photo-cathode. The benefit is that different types of PMT could use this model, without considering what the geometries look like.

![Fig. PMT geometry construction](figs/PMTmodel.png)

As this figure shows, PMT consists of two materials, Pyrex and Vacuum. The Pyrex part is main body. Then boolean operations are used to divide inner Vacuum into two part, Part I and Part II. Finally, we could get this optical surface.

To keep consistency, when construct geometries, we let equator is z=0. So the arrangement of PMTs will not change. Otherwise an additional shift is needed.

----
