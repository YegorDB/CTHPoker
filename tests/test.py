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
    ]

    @pytest.mark.parametrize("values", variants)
    @get_parameters
    def test_find_combo(self, cards, value):
        assert findCombo(cards) == value
