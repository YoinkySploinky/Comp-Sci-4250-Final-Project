# Comp-Sci-4250-Final-Project

Language compiler that will be given a language and then translate to ASM.

BNF grammar for the parser
S -> <A> <J>
A -> ^t1 <A> || Empty
C -> * <K>
D -> [ t1
E -> ] <K> ~
F -> | <G> ~
G -> <K> <K> <H> | <K>
H -> { || }
J -> <C> ~ <J> ~ <L> || <D> ~ <J> || <E> ~ <J> || t1 <F> ~ <J> || Empty
K -> t1 || t3
L -> <D> ~ || <E> || <F> ~ || Empty  

Each function will create a node for the nonTerminal i.e. <S>
then in the function it will assign the children of its node to the nonTerminals and terminals i.e. the <A> and <J> in <S>
If the node is discovered to be token1 or token3 it will then also assign the token to the child node 
