<h2>Infix to postfix expression converter</h2>
<p>This program converts between an infix expression to the equivalent postfix expression 
<br/>(also called reverse polish notation or RPN). This is useful especially because it's easier to
<br/>evaluate with a program (such as <a href="https://github.com/etrian-dev/rpn-calc">the one I wrote</a>).
<br/>
<blockquote>
	compile using gcc *.c -o execname
<br/>	To run it, just type ./execname "expr", where expr <b>MUST</b> be quoted, so that it's passed as argv[1]
</blockquote>
</p>
<ul>
	<li>Expressions can be written in the usual form, such as "1*3-(7+4/4)"</li>
	<li>Unary operators are not supported yet, so "-(-10)" would produce wrong RPN expressions</li>
	<li>Supported operations are the following: +, -, *, /, %, ^(<a href=#note_exp>note on exp</a>)</li>
</ul>
<p><span id="note_exp">Note:</span>The operator actually has left to right associativity, but parentheses can force certain patterns if needed</p>

