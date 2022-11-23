#include<iostream>
using namespace std;
int n, ans[20], acc;
void judge() {
	int sign = 1;
	int now = 0;
	int next = 1;
	for (int i = 1; i < n; i++) {
		if (ans[i] == 3) {
			next = next * 10 + (i + 1);

		}
		else {
			if (sign == 1) now += next;
			if (sign == 2) now -= next;
			if (ans[i] == 1) sign = 1;
			if (ans[i] == 2) sign = 2;
			next = i + 1;

		}

	}
	if (sign == 1) now += next;
	if (sign == 2) now -= next;
	if (!now) {
		for (int i = 1; i < n; i++) {
			cout << i;
			if (ans[i] == 1) cout << "+";
			if (ans[i] == 2) cout << "-";
			if (ans[i] == 3) cout << " ";

		}
		cout << n << endl;

	}

}
int dfs(int deep) {
	if (deep == n) {
		judge();
		return 0;

	}
	ans[deep] = 3;
	dfs(deep + 1);
	ans[deep] = 1;
	dfs(deep + 1);
	ans[deep] = 2;
	dfs(deep + 1);

}
int main() {
	cin >> n;
	dfs(1);
	return 0;

}
