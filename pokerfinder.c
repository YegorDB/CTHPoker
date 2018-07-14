#include <stdio.h>


int * sortNumbers(int *numbers, int count) {
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
	int repeats[7][7], repeatsCount[7], *all, allUnique[7], uniqueCount, max;
};


struct RepeatsData getRepeats(int *numbers, int count) {
	struct RepeatsData repeatsData;
	repeatsData.all = numbers;
	for (int i = 0; i < 7; i++) {
		repeatsData.repeatsCount[i] = 0;
	}
	repeatsData.max = 1;
	repeatsData.uniqueCount = 0;
	int newNumbers[7] = { 1, 1, 1, 1, 1, 1, 1 };
	for (int i = 0; i < count; i++) {
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
	//for (int i = 0; i < count; i++) {
	//	printf("number: %d, weight: %d, suit: %d\n", numbers[i], weights[i], suits[i]);
	//}
	//for (int i = 0; i < count; i++) {
	//	printf("%d weight repeat count %d: ", i + 1, repetitions.weights.repeatsCount[i]);
	//	for (int j = 0; j < repetitions.weights.repeatsCount[i]; j++) {
	//		printf("%d, ", repetitions.weights.repeats[i][j]);
	//	}
	//	printf("\n");
	//	printf("%d suit repeat count %d: ", i + 1, repetitions.suits.repeatsCount[i]);
	//	for (int j = 0; j < repetitions.suits.repeatsCount[i]; j++) {
	//		printf("%d, ", repetitions.suits.repeats[i][j]);
	//	}
	//	printf("\n");
	//}
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


struct Combo comboFinder(int *numbers, int count) {
	int value;
	int *sortedNumbers = sortNumbers(numbers, count);
	//for (int i = 0; i < count; i++) {
	//	printf("%d, ", sortedNumbers[i]);
	//}
	//printf("\n");
	struct Repetitions repetitions = getRepetitions(sortedNumbers, count);
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
			//printf("%d\n", straightMax);
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
			printf("straight flush");
			combo.type = 9;
			combo.itemsCount = 1;
			combo.items[0] = straightMax;
		}
		else {
			printf("flush");
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


int main(void) {
	//int nmb[] = { 131, 74, 121, 141, 91, 111, 101 }; // straight flush
	//int nmb[] = { 131, 74, 32, 71, 72, 103, 73 }; // four of a kind
	//int nmb[] = { 123, 52, 24, 124, 53, 51, 112 }; // full house
	//int nmb[] = { 44, 84, 132, 94, 31, 74, 114 }; // flush
	int nmb[] = { 53, 74, 83, 141, 91, 112, 101 }; // straight
	//int nmb[] = { 61, 104, 124, 101, 103, 143, 92 }; // three of a kind
	//int nmb[] = { 82, 123, 72, 54, 71, 23, 83 }; // two pairs
	//int nmb[] = { 64, 142, 114, 144, 31, 93, 122 }; // one pair
	//int nmb[] = { 43, 124, 62, 114, 51, 103, 83 }; // high card
	struct Combo combo = comboFinder(nmb, 7);
	for (int i = 0; i < combo.itemsCount; i++) {
		printf("%d, ", combo.items[i]);
	}
	printf("\n");
	return 0;
}