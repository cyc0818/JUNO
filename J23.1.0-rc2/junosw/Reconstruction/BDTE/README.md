# BDTE Reco

This algorthim have been developed by [Arsenii Gavrikov](gavrikov.997755@gmail.com). It's an implementation of the bdt desvelopped in the repo [J21_SYSU](https://github.com/ArseniiGav/J21_SYSU)

C++ implementation by [Leonard Imbert](imbert@subatech.in2p3.fr)

## Usage of the datastore

This algorithm is subdivided in three sub algorithm:
 - EDMReader : Will fetch the event from the EDM, calculate the features and store them in the python datastore - each `execute` call
 - Reco: Encapsulate the bdt. Will read the features from the python datastore, and use the BDT depending of the selected mode
    - inference : Infer the energy from the features - each `execute` call
    - control : Produce control samples of the selected features from the datastore - once when calling `finalize`
    - training : Train a new model - once when calling `finalize`
 - EDMWriter : Will write the energy stored in the datastore - each `execute` call


In theory the datastore is flushed each on each `execute` call
