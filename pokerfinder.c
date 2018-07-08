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
	int repeats[7][7], repeatsCount[7];
};


struct RepeatsData getRepeats(int *numbers, int count) {
	struct RepeatsData repeatsData;
	for (int i = 0; i < 7; i++) {
		repeatsData.repeatsCount[i] = 0;
	}
	int newNumbers[7] = { 1, 1, 1, 1, 1, 1, 1 };
	for (int i = 0; i < count; i++) {
		if (newNumbers[i] == 1) {
			int rep = 0;
			for (int j = 0; j < count; j++) {
				if (newNumbers[j] == 1 && numbers[i] == numbers[j]) {
					rep++;
					newNumbers[j] = 0;
				}
			}
			repeatsData.repeats[rep - 1][repeatsData.repeatsCount[rep - 1]] = numbers[i];
			repeatsData.repeatsCount[rep - 1]++;
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
	for (int i = 0; i < count; i++) {
		printf("number: %d, weight: %d, suit: %d\n", numbers[i], weights[i], suits[i]);
	}
	for (int i = 0; i < count; i++) {
		printf("%d weight repeat count %d: ", i + 1, repetitions.weights.repeatsCount[i]);
		for (int j = 0; j < repetitions.weights.repeatsCount[i]; j++) {
			printf("%d, ", repetitions.weights.repeats[i][j]);
		}
		printf("\n");
		printf("%d suit repeat count %d: ", i + 1, repetitions.suits.repeatsCount[i]);
		for (int j = 0; j < repetitions.suits.repeatsCount[i]; j++) {
			printf("%d, ", repetitions.suits.repeats[i][j]);
		}
		printf("\n");
	}
	return repetitions;
}


int finder(int *numbers, int count) {
	int *sortedNumbers = sortNumbers(numbers, count);
	for (int i = 0; i < count; i++) {
		printf("%d, ", sortedNumbers[i]);
	}
	printf("\n");
	getRepetitions(sortedNumbers, count);
	return 0;
}


int main(void) {
	int nmb[] = { 22, 234, 23, 141, 111, 232, 21 };
	//int nmb[] = { 22, 32, 42, 52, 62, 72, 82 };
	finder(nmb, 7);
	return 0;
}