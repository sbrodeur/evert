# Copyright (c) 2017, Simon Brodeur
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
# 
#  - Redistributions of source code must retain the above copyright notice, 
#    this list of conditions and the following disclaimer.
#  - Redistributions in binary form must reproduce the above copyright notice, 
#    this list of conditions and the following disclaimer in the documentation 
#    and/or other materials provided with the distribution.
#  - Neither the name of the NECOTIS research group nor the names of its contributors 
#    may be used to endorse or promote products derived from this software 
#    without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
# POSSIBILITY OF SUCH DAMAGE.

import os
import logging
import scipy.signal
import scipy.io.wavfile
import numpy as np
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d as plt3d

from scipy import signal

from evert import Room, Polygon, Vector3, Source, Listener, PathSolution

logger = logging.getLogger(__name__)

CDIR = os.path.dirname(os.path.realpath(__file__))

class MaterialAbsorptionTable(object):
    
    # From: Auralization : fundamentals of acoustics, modelling, simulation, algorithms and acoustic virtual reality
    
    categories = ['hard surfaces', 'linings', 'glazing', 'wood', 
                  'floor coverings', 'curtains']
    frequencies = [125.0, 250.0, 500.0, 1000.0, 2000.0, 4000.0, 8000.0]
    
    materials = [[  # Massive constructions and hard surfaces 
                    "average",                                  # Walls, hard surfaces average (brick walls, plaster, hard floors, etc.) 
                    "walls rendered brickwork",                 # Walls, rendered brickwork
                    "rough concrete",                           # Rough concrete
                    "smooth unpainted concrete",                # Smooth unpainted concrete 
                    "rough lime wash",                          # Rough lime wash 
                    "smooth brickwork with flush pointing, painted", # Smooth brickwork with flush pointing, painted 
                    "smooth brickwork, 10 mm deep pointing, pit sand mortar", # Smooth brickwork, 10 mm deep pointing, pit sand mortar 
                    "brick wall, stuccoed with a rough finish", # Brick wall, stuccoed with a rough finish 
                    "ceramic tiles with a smooth surface",      # Ceramic tiles with a smooth surface 
                    "limestone walls",                          # Limestone walls 
                    "reverberation chamber walls",              # Reverberation chamber walls 
                    "concrete",                                 # Concrete floor 
                    "marble floor",                             # Marble floor 
                ],
                [   # Lightweight constructions and linings
                    "plasterboard on steel frame",              # 2 * 13 mm plasterboard on steel frame, 50 mm mineral wool in cavity, surface painted 
                    "wooden lining",                            # Wooden lining, 12 mm fixed on frame 
                ],
                [   # Glazing
                    "single pane of glass",                     # Single pane of glass, 3 mm                                                  
                    "glass window",                             # Glass window, 0.68 kg/m^2
                    "lead glazing",                             # Lead glazing
                    "double glazing, 30 mm gap",                # Double glazing, 2-3 mm glass,  > 30 mm gap 
                    "double glazing, 10 mm gap ",               # Double glazing, 2-3 mm glass,  10 mm gap 
                    "double glazing, lead on the inside",       # Double glazing, lead on the inside
                ],
                [   # Wood
                    "wood, 1.6 cm thick",                       # Wood, 1.6 cm thick,  on 4 cm wooden planks 
                    "thin plywood panelling",                   # Thin plywood panelling
                    "16 mm wood on 40 mm studs",                # 16 mm wood on 40 mm studs 
                    "audience floor",                           # Audience floor, 2 layers,  33 mm on sleepers over concrete 
                    "stage floor",                              # Wood, stage floor, 2 layers, 27 mm over airspace 
                    "solid wooden door",                        # Solid wooden door 
                ],
                [   # Floor coverings
                    "linoleum, asphalt, rubber, or cork tile on concrete", # Linoleum, asphalt, rubber, or cork tile on concrete 
                    "cotton carpet",                            # Cotton carpet 
                    "loop pile tufted carpet",                  # Loop pile tufted carpet, 1.4 kg/m^2, 9.5 mm pile height: On hair pad, 3.0kg/m^2
                    "thin carpet",                              # Thin carpet, cemented to concrete
                    "pile carpet bonded to closed-cell foam underlay", # 6 mm pile carpet bonded to closed-cell foam underlay 
                    "pile carpet bonded to open-cell foam underlay", # 6 mm pile carpet bonded to open-cell foam underlay 
                    "tufted pile carpet",                       # 9 mm tufted pile carpet on felt underlay
                    "needle felt",                              # Needle felt 5 mm stuck to concrete 
                    "soft carpet",                              # 10 mm soft carpet on concrete
                    "hairy carpet",                             # Hairy carpet on 3 mm felt 
                    "rubber carpet",                            # 5 mm rubber carpet on concrete 
                    "carpet on hair felt or foam rubber",       # Carpet 1.35 kg/m^2, on hair felt or foam rubber 
                    "cocos fibre roll felt",                    # Cocos fibre roll felt, 29 mm thick (unstressed), reverse side clad  with paper, 2.2kg/m^2, 2 Rayl 
                ],
                [   # Curtains
                    "cotton curtains",                          # Cotton curtains (0.5 kg/m^2) draped to 3/4 area approx. 130 mm from wall
                    "curtains",                                 # Curtains (0.2 kg/m^2) hung 90 mm from wall 
                    "cotton cloth",                             # Cotton cloth (0.33 kg/m^2) folded to 7/8 area 
                    "densely woven window curtains",            # Densely woven window curtains 90 mm from wall 
                    "vertical blinds, half opened",             # Vertical blinds, 15 cm from wall,   half opened (45 deg) 
                    "vertical blinds, open",                    # Vertical blinds, 15 cm from wall,   open (90 deg) 
                    "tight velvet curtains",                    # Tight velvet curtains 
                    "curtain fabric",                           # Curtain fabric, 15 cm from wall 
                    "curtain fabric, folded",                   # Curtain fabric, folded, 15 cm from wall
                    "curtain of close-woven glass mat",         # Curtains of close-woven glass mat   hung 50 mm from wall 
                    "studio curtain",                           # Studio curtains, 22 cm from wall 
                ],
            ]
    
    # Tables of random-incidence absorption coefficients
    table = [   [   # Massive constructions and hard surfaces 
                    [0.02, 0.02, 0.03, 0.03, 0.04, 0.05, 0.05], # Walls, hard surfaces average (brick walls, plaster, hard floors, etc.) 
                    [0.01, 0.02, 0.02, 0.03, 0.03, 0.04, 0.04], # Walls, rendered brickwork
                    [0.02, 0.03, 0.03, 0.03, 0.04, 0.07, 0.07], # Rough concrete
                    [0.01, 0.01, 0.02, 0.02, 0.02, 0.05, 0.05], # Smooth unpainted concrete 
                    [0.02, 0.03, 0.04, 0.05, 0.04, 0.03, 0.02], # Rough lime wash 
                    [0.01, 0.01, 0.02, 0.02, 0.02, 0.02, 0.02], # Smooth brickwork with flush pointing, painted 
                    [0.08, 0.09, 0.12, 0.16, 0.22, 0.24, 0.24], # Smooth brickwork, 10 mm deep pointing, pit sand mortar 
                    [0.03, 0.03, 0.03, 0.04, 0.05, 0.07, 0.07], # Brick wall, stuccoed with a rough finish 
                    [0.01, 0.01, 0.01, 0.02, 0.02, 0.02, 0.02], # Ceramic tiles with a smooth surface 
                    [0.02, 0.02, 0.03, 0.04, 0.05, 0.05, 0.05], # Limestone walls 
                    [0.01, 0.01, 0.01, 0.02, 0.02, 0.04, 0.04], # Reverberation chamber walls 
                    [0.01, 0.03, 0.05, 0.02, 0.02, 0.02, 0.02], # Concrete floor 
                    [0.01, 0.01, 0.01, 0.02, 0.02, 0.02, 0.02], # Marble floor 
                ],
                [   # Lightweight constructions and linings
                    [0.15, 0.10, 0.06, 0.04, 0.04, 0.05, 0.05], # 2 * 13 mm plasterboard on steel frame, 50 mm mineral wool in cavity, surface painted 
                    [0.27, 0.23, 0.22, 0.15, 0.10, 0.07, 0.06], # Wooden lining, 12 mm fixed on frame 
                ],
                [   # Glazing
                    [0.08, 0.04, 0.03, 0.03, 0.02, 0.02, 0.02], # Single pane of glass, 3 mm                                                  
                    [0.10, 0.05, 0.04, 0.03, 0.03, 0.03, 0.03], # Glass window,, 0.68 kg/m^2
                    [0.30, 0.20, 0.14, 0.10, 0.05, 0.05, 0.05], # Lead glazing
                    [0.15, 0.05, 0.03, 0.03, 0.02, 0.02, 0.02], # Double glazing, 2-3 mm glass,  > 30 mm gap 
                    [0.10, 0.07, 0.05, 0.03, 0.02, 0.02, 0.02], # Double glazing, 2-3 mm glass,  10 mm gap 
                    [0.15, 0.30, 0.18, 0.10, 0.05, 0.05, 0.05], # Double glazing, lead on the inside
                ],
                [   # Wood
                    [0.18, 0.12, 0.10, 0.09, 0.08, 0.07, 0.07], # Wood, 1.6 cm thick,  on 4 cm wooden planks 
                    [0.42, 0.21, 0.10, 0.08, 0.06, 0.06, 0.06], # Thin plywood panelling
                    [0.18, 0.12, 0.10, 0.09, 0.08, 0.07, 0.07], # 16 mm wood on 40 mm studs 
                    [0.09, 0.06, 0.05, 0.05, 0.05, 0.04, 0.04], # Audience floor, 2 layers,  33 mm on sleepers over concrete 
                    [0.10, 0.07, 0.06, 0.06, 0.06, 0.06, 0.06], # Wood, stage floor, 2 layers, 27 mm over airspace 
                    [0.14, 0.10, 0.06, 0.08, 0.10, 0.10, 0.10], # Solid wooden door 
                ],
                [   # Floor coverings
                    [0.02, 0.03, 0.03, 0.03, 0.03, 0.02, 0.02], # Linoleum, asphalt, rubber, or cork tile on concrete 
                    [0.07, 0.31, 0.49, 0.81, 0.66, 0.54, 0.48], # Cotton carpet 
                    [0.10, 0.40, 0.62, 0.70, 0.63, 0.88, 0.88], # Loop pile tufted carpet, 1.4 kg/m^2, 9.5 mm pile height: On hair pad, 3.0kg/m^2
                    [0.02, 0.04, 0.08, 0.20, 0.35, 0.40, 0.40], # Thin carpet, cemented to concrete
                    [0.03, 0.09, 0.25, 0.31, 0.33, 0.44, 0.44], # 6 mm pile carpet bonded to closed-cell foam underlay 
                    [0.03, 0.09, 0.20, 0.54, 0.70, 0.72, 0.72], # 6 mm pile carpet bonded to open-cell foam underlay 
                    [0.08, 0.08, 0.30, 0.60, 0.75, 0.80, 0.80], # 9 mm tufted pile carpet on felt underlay
                    [0.02, 0.02, 0.05, 0.15, 0.30, 0.40, 0.40], # Needle felt 5 mm stuck to concrete 
                    [0.09, 0.08, 0.21, 0.26, 0.27, 0.37, 0.37], # 10 mm soft carpet on concrete
                    [0.11, 0.14, 0.37, 0.43, 0.27, 0.25, 0.25], # Hairy carpet on 3 mm felt 
                    [0.04, 0.04, 0.08, 0.12, 0.10, 0.10, 0.10], # 5 mm rubber carpet on concrete 
                    [0.08, 0.24, 0.57, 0.69, 0.71, 0.73, 0.73], # Carpet 1.35 kg/m^2, on hair felt or foam rubber 
                    [0.10, 0.13, 0.22, 0.35, 0.47, 0.57, 0.57], # Cocos fibre roll felt, 29 mm thick (unstressed), reverse side clad  with paper, 2.2kg/m^2, 2 Rayl 
                ],
                [   # Curtains
                    [0.30, 0.45, 0.65, 0.56, 0.59, 0.71, 0.71], # Cotton curtains (0.5 kg/m^2) draped to 3/4 area approx. 130 mm from wall
                    [0.05, 0.06, 0.39, 0.63, 0.70, 0.73, 0.73], # Curtains (0.2 kg/m^2) hung 90 mm from wall 
                    [0.03, 0.12, 0.15, 0.27, 0.37, 0.42, 0.42], # Cotton cloth (0.33 kg/m^2) folded to 7/8 area 
                    [0.06, 0.10, 0.38, 0.63, 0.70, 0.73, 0.73], # Densely woven window curtains 90 mm from wall 
                    [0.03, 0.09, 0.24, 0.46, 0.79, 0.76, 0.76], # Vertical blinds, 15 cm from wall,   half opened (45 deg) 
                    [0.03, 0.06, 0.13, 0.28, 0.49, 0.56, 0.56], # Vertical blinds, 15 cm from wall,   open (90 deg) 
                    [0.05, 0.12, 0.35, 0.45, 0.38, 0.36, 0.36], # Tight velvet curtains 
                    [0.10, 0.38, 0.63, 0.52, 0.55, 0.65, 0.65], # Curtain fabric, 15 cm from wall 
                    [0.12, 0.60, 0.98, 1.00, 1.00, 1.00, 1.00], # Curtain fabric, folded, 15 cm from wall
                    [0.03, 0.03, 0.15, 0.40, 0.50, 0.50, 0.50], # Curtains of close-woven glass mat   hung 50 mm from wall 
                    [0.36, 0.26, 0.51, 0.45, 0.62, 0.76, 0.76], # Studio curtains, 22 cm from wall 
                ],
            ]
    
    @staticmethod
    def getAbsorptionCoefficients(category, material):
        
        category = category.lower().strip()
        if category not in MaterialAbsorptionTable.categories:
            raise Exception('Unknown category for material absorption table: %s' % (category))
        categoryIdx = MaterialAbsorptionTable.categories.index(category)
        
        material = material.lower().strip()
        if material not in MaterialAbsorptionTable.materials[categoryIdx]:
            raise Exception('Unknown material for category %s in material absorption table: %s' % (category, material))
        materialIdx = MaterialAbsorptionTable.materials[categoryIdx].index(material)
        
        coefficients = np.array(MaterialAbsorptionTable.table[categoryIdx][materialIdx])
        frequencies = np.array(AirAttenuationTable.frequencies)
        
        eps = np.finfo(np.float).eps
        coefficientsDb = 20.0 * np.log10(1.0 - coefficients + eps)
        
        return coefficientsDb, frequencies
    
class AirAttenuationTable(object):
    
    # From: Auralization : fundamentals of acoustics, modelling, simulation, algorithms and acoustic virtual reality
    
    temperatures = [10.0, 20.0]
    relativeHumidities = [40.0, 60.0, 80.0]
    frequencies = [125.0, 250.0, 500.0, 1000.0, 2000.0, 4000.0, 8000.0]
    
    # Air attenuation coefficient, in 10^-3 / m 
    table = [   [ # 10 deg C
                    [0.1, 0.2, 0.5, 1.1, 2.7, 9.4, 29.0],  # 30-50% hum
                    [0.1, 0.2, 0.5, 0.8, 1.8, 5.9, 21.1],  # 50-70% hum
                    [0.1, 0.2, 0.5, 0.7, 1.4, 4.4, 15.8],  # 70-90% hum
                ],
                [ # 20 deg C
                    [0.1, 0.3, 0.6, 1.0, 1.9, 5.8, 20.3],  # 30-50% hum
                    [0.1, 0.3, 0.6, 1.0, 1.7, 4.1, 13.5],  # 50-70% hum
                    [0.1, 0.3, 0.6, 1.1, 1.7, 3.5, 10.6],  # 70-90% hum
                ]
            ]
    
    @staticmethod
    def getAttenuations(distance, temperature, relativeHumidity):
        closestTemperatureIdx = np.argmin(np.sqrt((np.array(AirAttenuationTable.temperatures) - temperature)**2))
        closestHumidityIdx = np.argmin(np.sqrt((np.array(AirAttenuationTable.relativeHumidities) - relativeHumidity)**2))
        
        attenuations = np.array(AirAttenuationTable.table[closestTemperatureIdx][closestHumidityIdx])
        frequencies = np.array(AirAttenuationTable.frequencies)
        
        eps = np.finfo(np.float).eps
        attenuations = np.clip(distance * 1e-3 * attenuations, 0.0, 1.0 - eps)
        attenuationsDb = 20.0 * np.log10(1.0 - attenuations)
        
        return attenuationsDb, frequencies
    
class FilterBank(object):
    
    def __init__(self, n, centerFrequencies, samplingRate):
        self.n = n
        
        if n % 2 == 0:
            self.n = n + 1
            logger.warn('Length of the FIR filter adjusted to the next odd number to ensure symmetry: %d' % (self.n))
        else:
            self.n = n
            
        self.centerFrequencies = centerFrequencies
        self.samplingRate = samplingRate
    
        centerFrequencies = np.array(centerFrequencies, dtype=np.float)
        centerNormFreqs = centerFrequencies/(self.samplingRate/2.0)
        cutoffs = centerNormFreqs[:-1] + np.diff(centerNormFreqs)/2
        
        filters = []
        for i in range(len(centerFrequencies)):
            if i == 0:
                # Low-pass filter
                b = scipy.signal.firwin(self.n, cutoff=cutoffs[0], window='hamming')
            elif i == len(centerFrequencies) - 1:
                # High-pass filter
                b = scipy.signal.firwin(self.n, cutoff=cutoffs[-1], window = 'hamming', pass_zero=False)
            else:
                # Band-pass filter
                b = scipy.signal.firwin(self.n, [cutoffs[i-1], cutoffs[i]], pass_zero=False)
                
            filters.append(b)
        self.filters = np.array(filters)
    
    def getScaledImpulseResponse(self, scales=1):
        if not isinstance(scales, (list, tuple)):
            scales = scales * np.ones(len(self.filters))
        return np.sum(self.filters * scales[:, np.newaxis], axis=0)
        
    def display(self, scales=1, merged=False):
        # Adapted from: http://mpastell.com/2010/01/18/fir-with-scipy/
        
        if merged:
            b = self.getScaledImpulseResponse(scales)
            filters = [b]
        else:
            filters = np.copy(self.filters)
            if not isinstance(scales, (list, tuple)):
                scales = scales * np.ones(len(filters))
            filters *= scales[:,np.newaxis]
        
        fig = plt.figure(figsize=(8,6), facecolor='white', frameon=True)
        for b in filters:
            w,h = signal.freqz(b,1)
            h_dB = 20 * np.log10(abs(h))
            plt.subplot(211)
            plt.plot(w/max(w),h_dB)
            plt.ylim(-150, 5)
            plt.ylabel('Magnitude (db)')
            plt.xlabel(r'Normalized Frequency (x$\pi$rad/sample)')
            plt.title(r'Frequency response')
            plt.subplot(212)
            h_Phase = np.unwrap(np.arctan2(np.imag(h),np.real(h)))
            plt.plot(w/max(w),h_Phase)
            plt.ylabel('Phase (radians)')
            plt.xlabel(r'Normalized Frequency (x$\pi$rad/sample)')
            plt.title(r'Phase response')
            plt.subplots_adjust(hspace=0.5)
        return fig
    
class EvertAcousticRoom(Room):

    materials = [   # category,         # material name          # index
                    ['hard surfaces',   'average'           ],   #   0
                    ['hard surfaces',   'concrete'          ],   #   1
                    ['glazing',         'glass window'      ],   #   2
                    ['wood',            'wood, 1.6 cm thick'],   #   3
                    ['floor coverings', 'linoleum'          ],   #   4
                    ['floor coverings', 'soft carpet'       ],   #   5
                    ['curtains',        'cotton curtains'   ],   #   6
                ]

    def __init__(self, samplingRate=16000, maximumOrder=3, 
                 materialAbsorption=True, frequencyDependent=True):

        super(EvertAcousticRoom,self).__init__()

        self.samplingRate = samplingRate
        self.maximumOrder = maximumOrder
        self.materialAbsorption = materialAbsorption
        self.frequencyDependent = frequencyDependent
        
        self.filterbank = FilterBank(n=513, 
                                     centerFrequencies=MaterialAbsorptionTable.frequencies,
                                     samplingRate=samplingRate)
        self.setAirConditions()
        
    def getMaterialIdByName(self, name):
        idx = None
        for i, (_, materialName) in enumerate(EvertAcousticRoom.materials):
            if materialName == name:
                idx = i
                break
        if idx is None:
            raise Exception('Unknown material %s' % (name))
        return idx
        
    def setAirConditions(self, temperature=20.0, relativeHumidity=65.0):
        self.temperature = temperature
        self.relativeHumidity = relativeHumidity

    def _calculateSoundSpeed(self):
        # Approximate speed of sound in dry (0% humidity) air, in meters per second, at temperatures near 0 deg C
        return 331.3*np.sqrt(1+self.temperature/273.15)
    
    def _calculateDelayAndAttenuation(self, path):
        
        # Calculate path length and corresponding delay
        pathLength = 0.0
        lastPt = path.m_points[0]
        for pt in path.m_points[1:]:
            pathLength += np.sqrt((lastPt.x - pt.x)**2 + 
                                  (lastPt.y - pt.y)**2 + 
                                  (lastPt.z - pt.z)**2)
            lastPt = pt
        pathLength = pathLength / 1000.0 # mm to m
        delay = pathLength/self._calculateSoundSpeed()
        
        # Calculate air attenuation coefficient (dB)
        airAttenuations, frequencies = AirAttenuationTable.getAttenuations(pathLength, self.temperature, self.relativeHumidity)

        # Calculate spherical geometric spreading attenuation (dB)
        distanceAttenuations = 20.0 * np.log10(1.0/pathLength)
        
        # Calculat material attenuation (dB)
        materialAttenuations = np.zeros((len(MaterialAbsorptionTable.frequencies),))
        if self.materialAbsorption:
            for polygon in path.m_polygons:
                materialId = polygon.getMaterialId()
                category, material = EvertAcousticRoom.materials[materialId]
                materialAbsoption, _ = MaterialAbsorptionTable.getAbsorptionCoefficients(category, material)
                materialAttenuations += materialAbsoption
        
        # Total attenuation (dB)
        attenuation = airAttenuations + distanceAttenuations + materialAttenuations
         
        return delay, attenuation, frequencies
    
    def calculateImpulseResponse(self, solution, maxImpulseLength=1.0, threshold=120.0, pathFilter=None):
    
        impulse = np.zeros((int(maxImpulseLength * self.samplingRate),))
        realImpulseLength = 0.0
        for i in range(solution.numPaths()):
            path = solution.getPath(i)
            delay, attenuationsDb, _ = self._calculateDelayAndAttenuation(path)
            
            if pathFilter is not None and not pathFilter(path): continue
            
            # Add path impulse to global impulse
            delaySamples = int(delay * self.samplingRate)
            
            # Skip paths that are below attenuation threshold (dB)
            if np.any(abs(attenuationsDb) < threshold):
                
                if self.frequencyDependent:
                
                    # Skip paths that would have their impulse responses truncated at the end
                    if delaySamples + self.filterbank.n < len(impulse):
                    
                        linearGains = 1.0/np.exp(-attenuationsDb/20.0 * np.log(10.0))
                        pathImpulse = self.filterbank.getScaledImpulseResponse(linearGains)
                            
                        # Random phase inversion
                        if np.random.random() > 0.5:
                            pathImpulse *= -1
                            
                        startIdx = delaySamples - self.filterbank.n/2
                        endIdx = startIdx + self.filterbank.n - 1
                        if startIdx < 0:
                            trimStartIdx = -startIdx
                            startIdx = 0
                        else:
                            trimStartIdx = 0
                        
                        impulse[startIdx:endIdx+1] += pathImpulse[trimStartIdx:]
                    
                        if endIdx+1 > realImpulseLength:
                            realImpulseLength = endIdx+1
                else:
                    # Use attenuation at 1000 Hz
                    linearGain = 1.0/np.exp(-attenuationsDb[3]/20.0 * np.log(10.0))
                    
                    # Random phase inversion
                    sign = 1.0
                    if np.random.random() > 0.5:
                        sign *= -1
                        
                    impulse[delaySamples] += linearGain * sign
                    if delaySamples+1 > realImpulseLength:
                        realImpulseLength = delaySamples+1
                
        return impulse[:realImpulseLength]

    def getSolutions(self):
        
        self.constructBSP()
        
        solutions = []
        for s in range(self.numSources()):
            for l in range(self.numListeners()):
                src = self.getSource(s)
                lst = self.getListener(l)
                solution = PathSolution(self, src, lst, self.maximumOrder)
                solution.update()
                solutions.append(solution)
        
        return solutions


if __name__ == '__main__':

    # Load audio file
    fs, signal = scipy.io.wavfile.read(os.path.join(CDIR, 'pop.wav'))
    fs = int(fs)
    signal = np.array(signal, dtype=np.float)
    signal /= np.max(np.abs(signal))
    t = np.arange(len(signal), dtype=np.float)/fs
    plt.figure()
    plt.title('Source signal')
    plt.plot(t, signal)
    plt.xlabel('Time [sec]')
    plt.ylabel('Amplitude')

    # Create acoustic environment
    room = EvertAcousticRoom(samplingRate=fs, maximumOrder=10, materialAbsorption=True, frequencyDependent=True)
    
    # Define a simple rectangular (length x width x height) as room geometry, with average hard-surface walls
    length = 20000  # mm
    width =  10000  # mm
    height = 2500   # mm
    face1poly = Polygon([Vector3(0,0,0), Vector3(0,width,0), Vector3(length,width,0), Vector3(length,0,0)])
    face2poly = Polygon([Vector3(0,0,0), Vector3(0,width,0), Vector3(0,width,height), Vector3(0,0,height)])
    face3poly = Polygon([Vector3(0,0,0), Vector3(length,0,0), Vector3(length,0,height), Vector3(0,0,height)])
    face4poly = Polygon([Vector3(0,0,height), Vector3(0,width,height), Vector3(length,width,height), Vector3(length,0,height)])
    face5poly = Polygon([Vector3(0,width,height), Vector3(0,width,0), Vector3(length,width,0), Vector3(length,width,height)])
    face6poly = Polygon([Vector3(length,0,height), Vector3(length,width,height), Vector3(length,width,0), Vector3(length,0,0)])
    roomPolygons = [face1poly, face2poly, face3poly, face4poly, face5poly, face6poly]
    for polygon in roomPolygons:
        polygon.setMaterialId(room.getMaterialIdByName('cotton curtains'))
        room.addPolygon(polygon, color=Vector3(0.5,0.5,0.5))
    
    # Print some room information
    center = room.getCenter()
    print 'Room maximum length: ', room.getMaxLength()
    print 'Room center: x=%f, y=%f, z=%f' % (center.x, center.y, center.z)
    print 'Number of elements: ', room.numElements()
    print 'Number of convex elements: ', room.numConvexElements()
    
    # Display room layout
    ax = plt3d.Axes3D(plt.figure())
    for polygon in roomPolygons:
        vtx = []
        for i in range(polygon.numPoints()):
            vtx.append([polygon[i].x, polygon[i].y, polygon[i].z])
        vtx = np.array(vtx)
        tri = plt3d.art3d.Poly3DCollection([vtx])
        tri.set_color([0.5, 0.5, 0.5, 0.25])
        tri.set_edgecolor('k')
        ax.add_collection3d(tri)
        ax.set_xlim(0, length)
        ax.set_ylim(0, width)
        ax.set_zlim(0, height) 
        ax.set_xlabel("x axis [mm]")
        ax.set_ylabel("y axis [mm]")
        ax.set_zlabel("z axis [mm]")
        ax.invert_xaxis()
        ax.invert_yaxis()
    
    # Define a simple pyramidal object in the middle of the room, with concrete walls
    width = 5000    # mm
    height = 1500   # mm
    pos = Vector3(7500, 5000, 1000)
    p = Vector3(pos.x-width/2, pos.y-width/2, pos.z-height/2)
    face1poly = Polygon([Vector3(p.x,p.y,p.z), Vector3(p.x,p.y+width,p.z), Vector3(p.x+width,p.y+width,p.z), Vector3(p.x+width,p.y,p.z)])
    face2poly = Polygon([Vector3(p.x,p.y,p.z), Vector3(p.x,p.y+width,p.z), Vector3(p.x+width/2,p.y+width/2,p.z+height)])
    face3poly = Polygon([Vector3(p.x,p.y,p.z), Vector3(p.x+width,p.y,p.z), Vector3(p.x+width/2,p.y+width/2,p.z+height)])
    face4poly = Polygon([Vector3(p.x,p.y+width,p.z), Vector3(p.x+width,p.y+width,p.z), Vector3(p.x+width/2,p.y+width/2,p.z+height)])
    face5poly = Polygon([Vector3(p.x+width,p.y,p.z), Vector3(p.x+width,p.y+width,p.z), Vector3(p.x+width/2,p.y+width/2,p.z+height)])
    objectPolygons = [face1poly, face2poly, face3poly, face4poly, face5poly]
    for polygon in objectPolygons:
        polygon.setMaterialId(room.getMaterialIdByName('concrete'))
        room.addPolygon(polygon, color=Vector3(0.5,0.5,0.5))
    
    # Display object layout
    for polygon in objectPolygons:
        vtx = []
        for i in range(polygon.numPoints()):
            vtx.append([polygon[i].x, polygon[i].y, polygon[i].z])
        vtx = np.array(vtx)
        tri = plt3d.art3d.Poly3DCollection([vtx])
        tri.set_color([1.0, 0.0, 0.0, 0.25])
        tri.set_edgecolor('k')
        ax.add_collection3d(tri)
        
    # Create binaural listener localized in room
    interauralDistance = 300 # mm
    list1 = Listener()
    list1.setPosition(Vector3(1500, 5000 - interauralDistance/2, 1000))
    list1.setName('Lst-left')
    room.addListener(list1)
    list2 = Listener()
    list2.setPosition(Vector3(1500, 5000 + interauralDistance/2, 1000))
    list2.setName('Lst-right')
    room.addListener(list2)

    # Create source localized between the listeners
    src1 = Source()
    src1.setPosition(Vector3(1500,5000,1000))
    src1.setName('Src')
    room.addSource(src1)

    # Display listeners and source layout
    ax.plot([list1.getPosition().x], [list1.getPosition().y], [list1.getPosition().z], color='k', marker='o')
    ax.plot([list2.getPosition().x], [list2.getPosition().y], [list2.getPosition().z], color='k', marker='o')
    ax.plot([src1.getPosition().x], [src1.getPosition().y], [src1.getPosition().z], color='r', marker='o')
    
    # Only consider reverberation paths that actually hit the object
    def mustHitObjectFilter(path):
        hitObject = False
        for polygon in path.m_polygons:
            materialId = polygon.getMaterialId()
            if materialId == room.getMaterialIdByName('concrete'):
                hitObject = True
                break
        return hitObject
    
    # Only consider non-direct reverberation paths
    def noDirectFilter(path):
        return len(path.m_polygons) > 0
    
    # Compute the reverberation paths
    solutions = room.getSolutions()
    impulseLeft = room.calculateImpulseResponse(solutions[0], maxImpulseLength=1.0, threshold=120.0, pathFilter=noDirectFilter)
    impulseRight = room.calculateImpulseResponse(solutions[1], maxImpulseLength=1.0, threshold=120.0, pathFilter=noDirectFilter)
    maxImpulseLength = max(len(impulseLeft), len(impulseRight))
    impulseLeft = np.pad(impulseLeft, (0, max(0, maxImpulseLength - len(impulseLeft))), mode='constant')
    impulseRight = np.pad(impulseRight, (0, max(0, maxImpulseLength - len(impulseRight))), mode='constant')
    
    # Diplay impulse response
    fig = plt.figure()
    plt.title('Estimated impulse responses')
    t = np.arange(len(impulseLeft), dtype=np.float)/room.samplingRate
    plt.plot(t, impulseLeft, color='b', label='Left channel')
    t = np.arange(len(impulseRight), dtype=np.float)/room.samplingRate
    plt.plot(t, impulseRight, color='g', label='Right channel')
    plt.legend()
    plt.xlabel('Time [sec]')
    plt.ylabel('Amplitude')
    
    # Apply impulse responses to click signal
    outLeft = np.convolve(signal, impulseLeft, mode='full')
    outRight = np.convolve(signal, impulseRight, mode='full')
    plt.figure()
    plt.title('Output signal')
    t = np.arange(len(outLeft), dtype=np.float)/fs
    plt.plot(t, outLeft, color='b', label='Left channel')
    t = np.arange(len(outRight), dtype=np.float)/fs
    plt.plot(t, outRight, color='g', label='Right channel')
    plt.xlabel('Time [sec]')
    plt.ylabel('Amplitude')
    plt.legend()
    
    # Save output sound to wav file
    outSignal = np.stack((outLeft, outRight), axis=1)
    outSignal /= np.max(np.abs(outSignal))
    scipy.io.wavfile.write(os.path.join(CDIR, 'output.wav'), fs, outSignal)
    
    # Wait until all figures are closed
    plt.show()
