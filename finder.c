#include <Python.h>


int * sortCards(int *numbers, int count) {
	static int result[7];
	result[0] = numbers[0];
	for (int i = 1; i < count; i++) {
		int isMax = 1;
		for (int j = 0; j < i; j++) {
			if (numbers[i] < result[j]) {
				isMax = 0;
				int val = numbers[i];
				for (int k = j; k <= i; k++) {
					int tmpVal = result[k];
					result[k] = val;
					val = tmpVal;
				}
				break;
			}
		}
		if (isMax == 1) {
			result[i] = numbers[i];
		}
	}
	return result;
}


struct Properties {
	int weight, suit;
};


struct Properties getProperties(int number) {
	struct Properties properties;
	properties.weight = (int)(number / 10);
	properties.suit = number - properties.weight * 10;
	return properties;
}


struct RepeatsData {
	int repeats[7][7], repeatsCount[7], all[7], allUnique[7], uniqueCount, max;
};


struct RepeatsData getRepeats(int *numbers, int count) {
	struct RepeatsData repeatsData;
	for (int i = 0; i < 7; i++) {
		repeatsData.repeatsCount[i] = 0;
	}
	repeatsData.max = 1;
	repeatsData.uniqueCount = 0;
	int newNumbers[7] = { 1, 1, 1, 1, 1, 1, 1 };
	for (int i = 0; i < count; i++) {
		repeatsData.all[i] = numbers[i];
		if (newNumbers[i] == 1) {
			repeatsData.allUnique[repeatsData.uniqueCount] = numbers[i];
			repeatsData.uniqueCount++;
			int rep = 0;
			for (int j = 0; j < count; j++) {
				if (newNumbers[j] == 1 && numbers[i] == numbers[j]) {
					rep++;
					newNumbers[j] = 0;
				}
			}
			repeatsData.repeats[rep - 1][repeatsData.repeatsCount[rep - 1]] = numbers[i];
			repeatsData.repeatsCount[rep - 1]++;
			if (rep > repeatsData.max) {
				repeatsData.max = rep;
			}
		}
	}
	return repeatsData;
}


struct Repetitions {
	struct RepeatsData weights, suits;
};


struct Repetitions getRepetitions(int *numbers, int count) {
	struct Repetitions repetitions;
	int weights[7], suits[7];
	for (int i = 0; i < count; i++) {
		struct Properties properties = getProperties(numbers[i]);
		weights[i] = properties.weight;
		suits[i] = properties.suit;
	}
	repetitions.weights = getRepeats(weights, count);
	repetitions.suits = getRepeats(suits, count);

	return repetitions;
}


int getStraightMax(int *numbers, int count) {
	if (numbers[count - 1] == 14) {
		int newNumbers[8];
		newNumbers[0] = 1;
		for (int i = 0; i < count; i++) {
			newNumbers[i + 1] = numbers[i];
		}
		numbers = newNumbers;
		count++;
	}

	int rank = 1;
	for (int i = count - 1; i > 0; i--) {
		if (numbers[i] - numbers[i - 1] == 1) {
			rank++;
			if (rank == 5) {
				return numbers[i + 3];
			}
		}
		else {
			rank = 1;
		}
	}

	return -1;
}


struct FilteredCards {
	int items[7], count;
};


struct FilteredCards getWeightsBySuit(int suit, int *allWeights, int *allSuits, int count) {
	struct FilteredCards filteredCards;
	filteredCards.count = 0;
	for (int i = 0; i < count; i++) {
		if (allSuits[i] == suit) {
			filteredCards.items[filteredCards.count] = allWeights[i];
			filteredCards.count++;
		}
	}

	return filteredCards;
}


struct Combo {
	int type, items[5], itemsCount;
};


struct Combo comboGetter(struct Repetitions repetitions, int count) {
	struct Combo combo;

	if (repetitions.suits.max < 5) {
		if (repetitions.weights.max == 4) {
			printf("four of a kind\n");
			combo.type = 8;
			combo.items[0] = repetitions.weights.repeats[3][0];
			if (count == 4) {
				combo.itemsCount = 1;
			}
			else {
				combo.itemsCount = 2;
				combo.items[1] = repetitions.weights.repeats[0][count - 5];
			}
		}
		else if (repetitions.weights.max == 3 && (repetitions.weights.repeatsCount[1] > 0 || repetitions.weights.repeatsCount[2] == 2)) {
			printf("full house\n");
			combo.type = 7;
			combo.itemsCount = 2;
			if (repetitions.weights.repeatsCount[2] == 2) {
				combo.items[0] = repetitions.weights.repeats[2][1];
				combo.items[1] = repetitions.weights.repeats[2][0];
			}
			else {
				combo.items[0] = repetitions.weights.repeats[2][0];
				combo.items[1] = repetitions.weights.repeats[1][repetitions.weights.repeatsCount[1] - 1];
			}
		}
		else {
			int straightMax = getStraightMax(repetitions.weights.allUnique, repetitions.weights.uniqueCount);
			if (straightMax != -1) {
				printf("straight\n");
				combo.type = 5;
				combo.itemsCount = 1;
				combo.items[0] = straightMax;
			}
			else if (repetitions.weights.max == 3) {
				printf("three of a kind\n");
				combo.type = 4;
				if (count < 5) {
					combo.itemsCount = count - 2;
				}
				else {
					combo.itemsCount = 3;
				}
				combo.items[0] = repetitions.weights.repeats[2][0];
				for (int i = 1; i < combo.itemsCount; i++) {
					combo.items[i] = repetitions.weights.repeats[0][count - i - 3];
				}
			}
			else if (repetitions.weights.repeatsCount[1] == 2) {
				printf("two pairs\n");
				combo.type = 3;
				combo.items[0] = repetitions.weights.repeats[1][1];
				combo.items[1] = repetitions.weights.repeats[1][0];
				if (count == 4) {
					combo.itemsCount = 2;
				}
				else {
					combo.itemsCount = 3;
					combo.items[2] = repetitions.weights.repeats[0][count - 5];
				}
			}
			else if (repetitions.weights.max == 2) {
				printf("one pair\n");
				combo.type = 2;
				if (count < 5) {
					combo.itemsCount = count - 1;
				}
				else {
					combo.itemsCount = 4;
				}
				combo.items[0] = repetitions.weights.repeats[1][0];
				for (int i = 1; i < combo.itemsCount; i++) {
					combo.items[i] = repetitions.weights.repeats[0][count - i - 2];
				}
			}
			else {
				printf("high card\n");
				combo.type = 1;
				if (count < 5) {
					combo.itemsCount = count;
				}
				else {
					combo.itemsCount = 5;
				}
				for (int i = 0; i < combo.itemsCount; i++) {
					combo.items[i] = repetitions.weights.repeats[0][count - i - 1];
				}
			}
		}
	}
	else {
		struct FilteredCards filteredCards = getWeightsBySuit(
			repetitions.suits.repeats[repetitions.suits.max - 1][0], repetitions.weights.all, repetitions.suits.all, count);
		int straightMax = getStraightMax(filteredCards.items, filteredCards.count);
		if (straightMax != - 1) {
			printf("straight flush\n");
			combo.type = 9;
			combo.itemsCount = 1;
			combo.items[0] = straightMax;
		}
		else {
			printf("flush\n");
			combo.type = 6;
			if (filteredCards.count < 5) {
				combo.itemsCount = filteredCards.count;
			}
			else {
				combo.itemsCount = 5;
			}
			for (int i = 0; i < combo.itemsCount; i++) {
				combo.items[i] = filteredCards.items[filteredCards.count - i - 1];
			}
		}
	}

	return combo;
}


struct Combo comboFinder(int *cards, int count) {
	int *sortedCards = sortCards(cards, count);
	struct Repetitions repetitions = getRepetitions(sortedCards, count);
	struct Combo combo = comboGetter(repetitions, count);

	return combo;
}


int * cardsFromRatio(int *ratioCards, int count) {
	static int cards[7];
	for (int i = 0; i < count; i++) {
		cards[i] = ratioCards[i] - ((int)(ratioCards[i] / 1000)) * 1000;
	}

	return cards;
}


int * getInHandCards(int *ratioCards, int count) {
	static int result[2], inCount = 0;
	for (int i = 0; i < count; i++) {
		if ((int)(ratioCards[i] / 1000) == 1) {
			result[inCount] = ratioCards[i] - ((int)(ratioCards[i] / 1000)) * 1000;
			inCount++;
			if (inCount == 2) {
				break;
			}
		}
	}

	return result;
}


struct RatioCombo {
	struct Combo value;
	int kind;
};


struct RatioCombo ratioComboFinder(int *ratioCards, int count) {
	struct RatioCombo ratioCombo;
	ratioCombo.kind = 0;
	int *cards = cardsFromRatio(ratioCards, count);
	int *sortedCards = sortCards(cards, count);
	struct Repetitions repetitions = getRepetitions(sortedCards, count);
	ratioCombo.value = comboGetter(repetitions, count);

	int *inHandCards = getInHandCards(ratioCards, count);

	if (ratioCombo.value.type == 9) {
		int suit = repetitions.suits.repeats[repetitions.suits.max - 1][0];
		for (int weight = ratioCombo.value.items[0]; weight > ratioCombo.value.items[0] - 5; weight--) {
			int card;
			if (weight == 1) {
				card = 140 + suit;
			}
			else {
				card = weight * 10 + suit;
			}
			if (card == inHandCards[0] || card == inHandCards[1]) {
				ratioCombo.kind = 2;
				break;
			}
		}
	}
	else if (ratioCombo.value.type == 8) {
		if (ratioCombo.value.items[0] == (int)(inHandCards[0] / 10) || ratioCombo.value.items[0] == (int)(inHandCards[1] / 10)) {
			ratioCombo.kind = 2;
		}
	}
	else if (ratioCombo.value.type == 7) {
		for (int i = 0; i < 2; i++) {
			int handCardWeight = (int)(inHandCards[i] / 10);
			if (ratioCombo.value.items[0] == handCardWeight || ratioCombo.value.items[1] == handCardWeight) {
				ratioCombo.kind++;
			}
		}
	}
	else if (ratioCombo.value.type == 6) {
		for (int i = 0; i < 5; i++) {
			if (ratioCombo.value.items[i] == (int)(inHandCards[0] / 10) || ratioCombo.value.items[i] == (int)(inHandCards[1] / 10)) {
				ratioCombo.kind = 2;
				break;
			}
		}
	}
	else if (ratioCombo.value.type == 5) {
		for (int weight = ratioCombo.value.items[0]; weight > ratioCombo.value.items[0] - 5; weight--) {
			if (weight == 1) {
				weight = 14;
			}
			if (weight == (int)(inHandCards[0] / 10) || weight == (int)(inHandCards[1] / 10)) {
				ratioCombo.kind = 2;
				break;
			}
		}
	}
	else if (ratioCombo.value.type == 4) {
		if (ratioCombo.value.items[0] == (int)(inHandCards[0] / 10) || ratioCombo.value.items[0] == (int)(inHandCards[1] / 10)) {
			ratioCombo.kind = 2;
		}
	}
	else if (ratioCombo.value.type == 3) {
		for (int i = 0; i < 2; i++) {
			int handCardWeight = (int)(inHandCards[i] / 10);
			if (ratioCombo.value.items[0] == handCardWeight || ratioCombo.value.items[1] == handCardWeight) {
				ratioCombo.kind++;
			}
		}
	}
	else if (ratioCombo.value.type == 2) {
		if (ratioCombo.value.items[0] == (int)(inHandCards[0] / 10) || ratioCombo.value.items[0] == (int)(inHandCards[1] / 10)) {
			ratioCombo.kind = 2;
		}
	}
	else {
		for (int i = 0; i < 5; i++) {
			if (ratioCombo.value.items[i] == (int)(inHandCards[0] / 10) || ratioCombo.value.items[i] == (int)(inHandCards[1] / 10)) {
				ratioCombo.kind = 2;
				break;
			}
		}
	}

	return ratioCombo;
}


PyObject* findCombo(PyObject* self, PyObject* args) {
    PyObject * cardsList;
    PyArg_ParseTuple(args, "O", &cardsList);
    int n = PyList_Size(cardsList), cardsArray[7];

    for (int i = 0; i < n; i++) {
        int card = PyLong_AsSsize_t(PyList_GetItem(cardsList, i));
        cardsArray[i] = card;
    }

    struct Combo combo = comboFinder(cardsArray, n);

    PyObject* result = PyList_New(combo.itemsCount + 1);
    PyList_SetItem(result, 0, Py_BuildValue("i", combo.type));

    for (int i = 1; i < combo.itemsCount + 1; i++) {
        PyList_SetItem(result, i, Py_BuildValue("i", combo.items[i - 1]));
    }

    return result;
}


PyObject* findRatioCombo(PyObject* self, PyObject* args) {
    PyObject * cardsList;
    PyArg_ParseTuple(args, "O", &cardsList);
    int n = PyList_Size(cardsList), cardsArray[7];

    for (int i = 0; i < n; i++) {
        int card = PyLong_AsSsize_t(PyList_GetItem(cardsList, i));
        cardsArray[i] = card;
    }

    struct RatioCombo ratioCombo = ratioComboFinder(cardsArray, n);

    PyObject* result = PyList_New(2);

    PyObject* comboList = PyList_New(ratioCombo.value.itemsCount + 1);
    PyList_SetItem(comboList, 0, Py_BuildValue("i", ratioCombo.value.type));
    for (int i = 1; i < ratioCombo.value.itemsCount + 1; i++) {
        PyList_SetItem(comboList, i, Py_BuildValue("i", ratioCombo.value.items[i - 1]));
    }

    PyList_SetItem(result, 0, comboList);
    PyList_SetItem(result, 1, Py_BuildValue("i", ratioCombo.kind));

    return result;
}


static PyMethodDef methods[] = {
    {"findCombo", findCombo, METH_VARARGS, "Find out combination kind."},
    {"findRatioCombo", findRatioCombo, METH_VARARGS, "Find out combination kind with ratio."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef definition = {
    PyModuleDef_HEAD_INIT,
    "cthpoker",
    "Texas Hold'em poker combination finder.",
    -1,
    methods
};


PyMODINIT_FUNC PyInit_cthpoker(void) {
    Py_Initialize();
    return PyModule_Create(&definition);
}
