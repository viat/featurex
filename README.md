featurex - Feature Extractor
============================
featureX integrates YapHash into the VIAT eco-system. 
YapHash (Yet another Perceptual Hash) is a perceptual fingerprint for audio identification purposes. 
Although YapHash has been developed for detecting telephone-spam and therefore optimized for speech content, 
it could also be used for music.  

Standard cryptographic hash functions have the property that minimal alterations of input data 
significantly change the resulting hash value. 
Robust audio hashes in contrast produce identical or nearby hash values for similar input data. 
In particular, the audio hashes should be invariant to (certain) content degradations and hence permit 
a perceptual audio comparison and an efficient identification of a specific recording. 
Different recordings with similar content, e.g. identical words or phrases, or from the same speaker are not identified.

The YapHash fingerprint is based on the spectrogram and Mel-frequency-cepstrum-coefficients (MFCC). 
The sign of the differences in between the frequency bands and the MFCCs form a compact yet robust audio hash.
The communication with other VIAT system components is done through tcp sockets. 
It receives the audio data from call extractor (callx) and sends the 
sends the resulting hash to search engine (mld).

###Usage

featureX is currently a command line tool and expects at least one argument; the configuration file e.g. _hash.cfg_.
Configuration (e.g. window size, ip addresses) is mostly done in this config file. 

Example:

    featureX hash.cfg
    