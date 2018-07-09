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
	int repeats[7][7], repeatsCount[7], allUnique[7], uniqueCount, max;
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


int getStraightIndex(int *numbers, int count) {
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
				return i - 1;
			}
		}
		else {
			rank = 1;
		}
	}

	return -1;
}


int finder(int *numbers, int count) {
	int value;
	int *sortedNumbers = sortNumbers(numbers, count);
	//for (int i = 0; i < count; i++) {
	//	printf("%d, ", sortedNumbers[i]);
	//}
	//printf("\n");
	struct Repetitions repetitions = getRepetitions(sortedNumbers, count);

	if (repetitions.suits.max < 5) {
		if (repetitions.weights.max == 4) {
			printf("four of a kind");
			value = 8;
		}
		else if (repetitions.weights.max == 3 && (repetitions.weights.repeatsCount[1] > 0 || repetitions.weights.repeatsCount[2] == 2)) {
			printf("full house");
			value = 7;
		}
		else {
			int straightIndex = getStraightIndex(repetitions.weights.allUnique, repetitions.weights.uniqueCount);
			printf("%d\n", straightIndex);
			//for (int i = 0; i < repetitions.weights.uniqueCount; i++) {
			//	printf("%d, ", repetitions.weights.allUnique[i]);
			//}
		}
	}

	return 0;
}


int main(void) {
	int nmb[] = { 53, 74, 83, 141, 91, 112, 101 };
	//int nmb[] = { 22, 32, 42, 52, 62, 72, 82 };
	finder(nmb, 7);
	return 0;
}