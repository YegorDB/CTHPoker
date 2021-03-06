# Copyright 2019 Yegor Bitensky

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


import pytest

from cthpoker import findCombo, findRatioCombo


def get_parameters(func):
    def wrap(self, values):
        return func(self, **values)
    return wrap


class TestFindCombo:
    variants = [
        {'cards': [144, 134, 124, 114, 104], 'value': [9, 14]},
        {'cards': [134, 122, 124, 114, 104, 94], 'value': [9, 13]},
        {'cards': [124, 114, 101, 104, 94, 84, 21], 'value': [9, 12]},
        {'cards': [123, 114, 104, 94, 84, 73, 74], 'value': [9, 11]},
        {'cards': [141, 104, 94, 84, 74, 64, 52], 'value': [9, 10]},
        {'cards': [94, 84, 74, 73, 64, 52, 54], 'value': [9, 9]},
        {'cards': [121, 84, 74, 64, 54, 44, 23], 'value': [9, 8]},
        {'cards': [143, 74, 64, 54, 44, 34, 23], 'value': [9, 7]},
        {'cards': [114, 104, 64, 54, 44, 34, 24], 'value': [9, 6]},
        {'cards': [141, 144, 62, 54, 44, 34, 24], 'value': [9, 5]},
        {'cards': [144, 142, 143, 141], 'value': [8, 14]},
        {'cards': [134, 144, 132, 133, 131], 'value': [8, 13, 14]},
        {'cards': [124, 134, 144, 122, 123, 121], 'value': [8, 12, 14]},
        {'cards': [114, 134, 112, 133, 113, 111], 'value': [8, 11, 13]},
        {'cards': [104, 144, 53, 102, 103, 101], 'value': [8, 10, 14]},
        {'cards': [144, 142, 143, 131, 133], 'value': [7, 14, 13]},
        {'cards': [144, 131, 133, 124, 123, 122], 'value': [7, 12, 13]},
        {'cards': [131, 114, 113, 133, 112, 134, 53], 'value': [7, 13, 11]},
        {'cards': [101, 114, 113, 103, 83, 104, 82], 'value': [7, 10, 11]},
        {'cards': [114, 51, 112, 92, 113, 54, 52], 'value': [7, 11, 5]},
        {'cards': [144, 134, 124, 114, 94], 'value': [6, 14, 13, 12, 11, 9]},
        {'cards': [143, 134, 104, 94, 54, 24], 'value': [6, 13, 10, 9, 5, 2]},
        {'cards': [132, 104, 94, 73, 74, 54, 24], 'value': [6, 10, 9, 7, 5, 2]},
        {'cards': [94, 84, 74, 54, 44, 34, 24], 'value': [6, 9, 8, 7, 5, 4]},
        {'cards': [123, 74, 54, 44, 34, 24, 21], 'value': [6, 7, 5, 4, 3, 2]},
        {'cards': [144, 133, 121, 112, 103], 'value': [5, 14]},
        {'cards': [121, 113, 104, 103, 94, 81], 'value': [5, 12]},
        {'cards': [104, 94, 81, 72, 64, 54, 23], 'value': [5, 10]},
        {'cards': [81, 72, 63, 54, 41, 32, 23], 'value': [5, 8]},
        {'cards': [114, 113, 64, 51, 42, 31, 23], 'value': [5, 6]},
        {'cards': [141, 51, 42, 31, 23], 'value': [5, 5]},
        {'cards': [144, 143, 141], 'value': [4, 14]},
        {'cards': [134, 133, 131, 42], 'value': [4, 13, 4]},
        {'cards': [142, 124, 123, 121, 72], 'value': [4, 12, 14, 7]},
        {'cards': [132, 122, 114, 113, 111, 92], 'value': [4, 11, 13, 12]},
        {'cards': [74, 52, 42, 34, 33, 31, 22], 'value': [4, 3, 7, 5]},
        {'cards': [144, 143, 131, 133], 'value': [3, 14, 13]},
        {'cards': [142, 131, 133, 123, 122], 'value': [3, 13, 12, 14]},
        {'cards': [132, 111, 113, 101, 93, 92], 'value': [3, 11, 9, 13]},
        {'cards': [101, 104, 93, 92, 74, 51, 53], 'value': [3, 10, 9, 7]},
        {'cards': [51, 53, 43, 42, 31, 34, 24], 'value': [3, 5, 4, 3]},
        {'cards': [144, 143], 'value': [2, 14]},
        {'cards': [144, 132, 133], 'value': [2, 13, 14]},
        {'cards': [134, 122, 123, 112], 'value': [2, 12, 13, 11]},
        {'cards': [134, 122, 113, 112, 104], 'value': [2, 11, 13, 12, 10]},
        {'cards': [143, 113, 104, 83, 81, 63], 'value': [2, 8, 14, 11, 10]},
        {'cards': [131, 122, 113, 104, 63, 51, 53], 'value': [2, 5, 13, 12, 11]},
        {'cards': [44], 'value': [1, 4]},
        {'cards': [141, 72], 'value': [1, 14, 7]},
        {'cards': [122, 53, 32], 'value': [1, 12, 5, 3]},
        {'cards': [112, 103, 91, 84], 'value': [1, 11, 10, 9, 8]},
        {'cards': [134, 102, 83, 63, 21], 'value': [1, 13, 10, 8, 6, 2]},
        {'cards': [104, 93, 72, 54, 43, 31], 'value': [1, 10, 9, 7, 5, 4]},
        {'cards': [134, 122, 94, 73, 52, 41, 23], 'value': [1, 13, 12, 9, 7, 5]},
    ]

    @pytest.mark.parametrize("values", variants)
    @get_parameters
    def test_all_values(self, cards, value):
        assert findCombo(cards) == value


class TestFindRatioCombo:
    variants = [
        {'cards': [114, 104, 94, 84, 21, 1124, 1101], 'value': [9, 12], 'kind': 2},
        {'cards': [104, 94, 84, 74, 64, 1141, 1051], 'value': [9, 10], 'kind': 0},
        {'cards': [143, 21, 31, 41, 51, 1141, 1082], 'value': [9, 5], 'kind': 2},
        {'cards': [141, 21, 31, 41, 51, 1142, 1134], 'value': [9, 5], 'kind': 0},
        {'cards': [134, 132, 133, 131, 74, 1144, 1022], 'value': [8, 13, 14], 'kind': 0},
        {'cards': [134, 144, 123, 121, 22, 1124, 1122], 'value': [8, 12, 14], 'kind': 2},
        {'cards': [112, 132, 113, 134, 52, 1131, 1114], 'value': [7, 13, 11], 'kind': 2},
        {'cards': [114, 112, 102, 104, 82, 1101, 1083], 'value': [7, 10, 11], 'kind': 1},
        {'cards': [114, 51, 112, 54, 52, 1092, 1134], 'value': [7, 5, 11], 'kind': 0},
        {'cards': [74, 54, 44, 34, 24, 1094, 1084], 'value': [6, 9, 8, 7, 5, 4], 'kind': 2},
        {'cards': [74, 54, 44, 34, 24, 1123, 1021], 'value': [6, 7, 5, 4, 3, 2], 'kind': 0},
        {'cards': [63, 54, 41, 32, 23, 1081, 1072], 'value': [5, 8], 'kind': 2},
        {'cards': [64, 51, 42, 31, 23, 1114, 1113], 'value': [5, 6], 'kind': 0},
        {'cards': [142, 24, 31, 41, 53, 1144, 1101], 'value': [5, 5], 'kind': 2},
        {'cards': [142, 24, 31, 41, 53, 1104, 1101], 'value': [5, 5], 'kind': 0},
        {'cards': [122, 113, 111, 92, 52, 1132, 1114], 'value': [4, 11, 13, 12], 'kind': 2},
        {'cards': [42, 34, 33, 31, 22, 1074, 1052], 'value': [4, 3, 7, 5], 'kind': 0},
        {'cards': [113, 132, 101, 92, 34, 1111, 1093], 'value': [3, 11, 9, 13], 'kind': 2},
        {'cards': [104, 93, 92, 74, 53, 1101, 1051], 'value': [3, 10, 9, 7], 'kind': 1},
        {'cards': [51, 53, 43, 42, 31, 1034, 1024], 'value': [3, 5, 4, 3], 'kind': 0},
        {'cards': [143, 112, 104, 63, 43, 1083, 1081], 'value': [2, 8, 14, 11, 10], 'kind': 2},
        {'cards': [112, 104, 63, 51, 53, 1131, 1123], 'value': [2, 5, 13, 12, 11], 'kind': 0},
        {'cards': [134, 122, 94, 73, 23, 1052, 1041], 'value': [1, 13, 12, 9, 7, 5], 'kind': 2},
        {'cards': [141, 104, 93, 72, 54, 1043, 1031], 'value': [1, 14, 10, 9, 7, 5], 'kind': 0},
    ]

    @pytest.mark.parametrize("values", variants)
    @get_parameters
    def test_all_values(self, cards, value, kind):
        assert findRatioCombo(cards) == [value, kind]
