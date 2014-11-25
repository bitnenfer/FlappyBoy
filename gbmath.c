#include <gb/gb.h>
#include <rand.h>

UWORD seed;
const WORD SIN_TABLE[] = {0, 841, 909, 141, -756, -958, -279, 656, 989, 412, -544, -999, -536, 420, 990, 650, -287, -961, -750, 149, 912, 836, -8, -846, -905, -132, 762, 956, 270, -663, -988, -404, 551, 999, 529, -428, -991, -643, 296, 963, 745, -158, -916, -831, 17, 850, 901, 123, -768, -953, -262, 670, 986, 395, -558, -999, -521, 436, 992, 636, -304, -966, -739, 167, 920, 826, -26, -855, -897, -114, 773, 951, 253, -676, -985, -387, 566, 999, 513, -444, -993, -629, 313, 968, 733, -176, -923, -821, 35, 860, 893, 105, -779, -948, -245, 683, 983, 379, -573, -999, -506, 452, 994, 622, -321, -970, -727, 184, 926, 816, -44, -864, -889, -97, 784, 945, 236, -689, -981, -371, 580, 998, 498, -459, -995, -616, 329, 972, 721, -193, -930, -811, 53, 868, 885, 88, -790, -942, -228, 696, 980, 363, -587, -998, -491, 467, 996, 609, -338, -974, -714, 202, 933, 806, -61, -873, -881, -79, 795, 939, 219, -702, -978, -354, 594, 997, 483, -475, -997, -601, 346, 976, 708, -210, -936, -801, 70, 877, 877, 70, -801, -936, -210, 708, 976, 346, -602, -997, -475, 483, 997, 594, -354, -978, -702, 219, 939, 795, -79, -881, -873, -61, 806, 933, 202, -714, -974, -338, 609, 996, 467, -491, -998, -587, 363, 980, 696, -228, -942, -790, 88, 885, 868, 53, -811, -930, -193, 721, 972, 329, -616, -995, -459, 498, 998, 580, -371, -981, -689, 236, 945, 784, -97, -890, -864, -44, 816, 926, 184, -727, -970, -321, 623, 994, 451, -506, -999, -573, 379, 983, 683, -245, -948, -779, 106, 894, 860, 35, -821, -923, -176, 733, 968, 313, -629, -993, -444, 514, 999, 566, -387, -985, -676, 253, 951, 773, -114, -897, -855, -26, 826, 920, 167, -739, -966, -304, 636, 992, 436, -521, -999, -558, 395, 986, 670, -262, -953, -768, 123, 901, 850, 17, -831, -916, -158, 745, 963, 296, -643, -991, -428, 529, 999, 551, -404, -988, -663, 270, 956, 762, -132, -905, -846, -8, 836, 912, 149, -751, -961, -287, 650, 990, 420, -536, -999, -543, 412, 989, 656, -279, -958, -756, 141, 909, 841, 0, -841, -909, -141, 756};
WORD sign (WORD x);
WORD sqrt(WORD num);
WORD clamp (WORD n, WORD min, WORD max);
WORD max (WORD x, WORD y);
WORD min (WORD x, WORD y);
WORD random (WORD value);
WORD randomRange (WORD min, WORD max);
WORD norm (WORD value, WORD min, WORD max);
WORD lerp (WORD norm, WORD value);
WORD lerpRange(WORD norm, WORD min, WORD max);
WORD sin(WORD x);
WORD cos(WORD x);

WORD sin (WORD x) {
	WORD xn = x  % 360;
	if (xn < 0) xn += 360;
	return SIN_TABLE[xn];
}

WORD cos (WORD x) {
	return sin(90 - x);
}

void initMath () {
	seed = DIV_REG;
    waitpadup();
    seed |= ((UINT16)DIV_REG << 8);
    initarand(seed);
}

WORD sign (WORD x) {
	if (x > 0) {
		return 1;
	} else {
		return -1;
	}
}

WORD sqrt(WORD num) {
    WORD res = 0;
    WORD bit = 1 << 8; 
    while (bit > num) {
        bit >>= 2;
	} 
    while (bit != 0) {
        if (num >= res + bit) {
            num -= res + bit;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
        bit >>= 2;
    }
    return res;
}

WORD clamp (WORD n, WORD min, WORD max) {
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

WORD max (WORD x, WORD y) {
	return x > y ? x : y;
}

WORD min (WORD x, WORD y) {
	return x < y ? x : y;
}

WORD random (WORD value) {
	return ((UWORD)rand() * value) >> 8;
}

WORD randomRange (WORD min, WORD max) {
	return min + random((max - min));
}

WORD norm (WORD value, WORD min, WORD max) {
	return ((value  - min ) << 8 / (max  - min ) << 8) >> 8;
}

WORD lerp (WORD norm, WORD value) {
	return value * norm / 10;
}

WORD lerpRange(WORD norm, WORD min, WORD max) {
	return lerp(norm, (max - min)) + min;
}