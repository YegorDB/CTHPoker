# Texas Hold'em Poker combination finder tool

## Install

```
pip install CTHPoker
```

## Usage

### Card

Card is a number.

Weight numbers: `20` (Two), `30` (Three), `40` (Four), `50` (Five), `60` (Six), `70` (Seven), `80` (Eight), `90` (Nine), `100` (Ten), `110` (Jack), `120` (Queen), `130` (King), `140` (Ace).

Suit numbers: `1` (clubs), `2` (diamonds), `3` (hearts), `4` (spades).

Ace of a spades looks like `144`.

### Combination

Combination is a list of numbers.

First number is type of combination: `1` (High card), `2` (One pair), `3` (Two pairs), `4` (Three of a kind), `5` (Straight), `6` (Flush), `7` (Full house), `8` (Four of a kind), `9` (StraightFlush).

Other numbers is combination value.

`[1, 12, 10, 7, 6, 4]` is high card of Queen with Ten, Seven, Six and Four
`[2, 9, 13, 8, 5]` is one pair of Nine with King, Eight and Five
`[3, 11, 3, 9]` is two pairs of Jakes and Three with Nine
`[4, 2, 14, 10]` is three of a kind of Deuce with Ace and Ten
`[5, 6]` is straight of Six, Five, Four, Three and Two
`[6, 10, 9, 6, 4, 2]` is flush of Ten, Nine, Six, Four and Two
`[7, 3, 6]` is full house of Three and Six
`[8, 7, 12]` is four of a kind of Seven with Queen
`[9, 10]` is straightFlush of Ten, Nine, Eight, Seven and Six

### Find pure combination

```python
>>> from cthpoker import findCombo

>>> cards = [144, 134, 124, 114, 104] # A♠ K♠ Q♠ J♠ T♠
>>> combo = findCombo(cards)
>>> combo
[9, 14]
```

### Ratio

Ratio shows whether player hand cards in combination base cards or not.

There is three ratio kind: `0` - nominal combination (player hand cards not in combination base cards), `1` - half nominal combination (player hand cards in part of combination base cards, possible in Full house and Two pairs only), `2` - real combination (player hand cards in combination base cards).

#### High card

Combination cards - `A♠ Q♦ T♠ 7♦ 6♣`. Combination base cards - `A♠ Q♦ T♠ 7♦ 6♣`. Other cards are absent.

#### One pair

Combination cards - `9♣ 9♥ K♦ Q♣ 7♠`. Combination base cards - `9♣ 9♥`. Other cards - `K♦ Q♣ 7♠`.

#### Two pairs

Combination cards - `J♥ J♦ 4♠ 4♦ 3♣`. Combination base cards - `J♥ J♦ 4♠ 4♦`. Other cards - `3♣`. There are two parts of combination base cards: first - `J♥ J♦` , second - `4♠ 4♦`.

#### Three of a kind

Combination cards - `6♦ 6♠ 6♣ Q♥ 8♠`. Combination base cards - `6♦ 6♠ 6♣`. Other cards - `Q♥ 8♠`.

#### Straight

Combination cards - `K♠ Q♦ J♠ T♦ 9♣`. Combination base cards - `K♠ Q♦ J♠ T♦ 9♣`. Other cards are absent.

#### Flush

Combination cards - `A♥ J♥ 8♥ 5♥ 2♥`. Combination base cards - `A♥ J♥ 8♥ 5♥ 2♥`. Other cards are absent.

#### Full house

Combination cards - `3♣ 3♦ 3♥ 8♣ 8♠`. Combination base cards - `3♣ 3♦ 3♥ 8♣ 8♠`. Other cards are absent. There are two parts of combination base cards: first - `3♣ 3♦ 3♥` , second - `8♣ 8♠`.

#### Four of a kind

Combination cards - `Q♠ Q♦ Q♣ Q♥ 4♣`. Combination base cards - `Q♠ Q♦ Q♣ Q♥`. Other cards - `4♣`.

#### StraightFlush

Combination cards - `A♠ K♠ Q♠ J♠ T♠`. Combination base cards - `A♠ K♠ Q♠ J♠ T♠`. Other cards are absent.



### Find ratio combination

	 In hand card is card increased by 1000

```python
>>> from cthpoker import findRatioCombo

>>> table_cards = [144, 84, 72, 54, 24] # A♠ 8♠ 7♦ 5♠ 2♠
>>> hand_cards = [1114, 1031] #  J♠ 3♣
>>> combo, ratio = findRatioCombo(table_cards + hand_cards)
>>> combo
[6, 14, 11, 8, 5, 2]
>>> ratio
2

>>> table_cards = [103, 102, 104, 133, 83] # T♥ T♦ T♠ K♥ 8♥
>>> hand_cards = [1131, 1041] # K♣ 4♣
>>> combo, ratio = findRatioCombo(table_cards + hand_cards)
>>> combo
[7, 10, 13]
>>> ratio
1

>>> table_cards = [52, 54, 51, 124, 101] # 5♦ 5♠ 5♣ Q♠ T♣
>>> hand_cards = [1062, 1023] # 6♦ 2♥
>>> combo, ratio = findRatioCombo(table_cards + hand_cards)
>>> combo
[4, 5, 12, 10]
>>> ratio
0
```

## License
[Apache License](https://choosealicense.com/licenses/apache-2.0/)
