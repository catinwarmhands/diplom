#import "../../modules/Print.jai"

fib :: (X: s64) -> s64 {
	if X < 2 return X
	return fib(X - 1) + fib(X - 2)
}

#run main()
main :: () {
	print("fib(%) = %\n", fib(28), 28)
}