package staptest;

class ClassUnloadedProbeTester {
	private int myField;
        long total;
	static ClassUnloadedProbeTester tester;

	public static void main(String[] args) {
		long total = 0;
		for (int i = 1; i < 105; i++) {
			tester = new ClassUnloadedProbeTester();
			tester.runner(tester, i);
			if (total != tester.total)
				total += tester.getField();
		}
	}

	void runner(ClassUnloadedProbeTester run, int j) {
		for (int i = 1; i < 105; i++) {
			run.setField(i);
			total += tester.getField();
		}
		total += j;
	}

	public ClassUnloadedProbeTester() {
		myField = 500;
	}

	public int getField() {
		return myField;
	}

	public void setField(int newValue) {
		myField = newValue;
	}
}

