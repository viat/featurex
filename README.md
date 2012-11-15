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
The communication with other VIAT system components is done through tcp sockets. It receives the audio data from callX and sends the 
sends the resulting hash to mlDaemon

###Usage

featureX is currently a command line tool and expects at least one argument; the configuration file e.g. _hash.cfg_.
Configuration is mostly done in this config file. 

Example:

    featureX hash.cfg
    


# Viat Feature Extractor Config File

DEBUG_LEVEL    		= 0

MAX_AUDIO_LEN 		= 6
MIN_AUDIO_LEN 		= 2
#other samplerates than 8kHz are not yet supported 
SAMPLERATE 			= 8000 

# Preemphasize higher frequencies, 0 is off, 99 is max (complies to 0.99 in code)
PREEMPHASIZE_FACTOR  = 0

# Voice Activity Detection Threshold (1-100)
VAD_THRESHOLD 		 = 20

# Threshold for declicker should be between 1 and 50
DECLICKER_THRESHOLD  = 40

# STFT
WINDOWSIZE            = 2960
STEPSIZE              = 94

# Hash
MAX_HASH_LEN          = 200
NUMBER_OF_MFCC        = 12
NUMBER_OF_MEL_COEFF   = 20
ENERGY_THRESHOLD      = 50

# Mel Filterbank
HICUT                 	= 1800
LOCUT                 	= 300

# Output
WRITE_TO_CSV			= 1
WRITE_TO_SOCKET			= 0

# Sockets
INDEXD_IP_ADDRESS   	= 139.6.237.45
INDEXD_PORT				= 3000
CALLX_PORT				= 3000