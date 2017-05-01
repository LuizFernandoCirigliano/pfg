library(ggplot2)
library(gridExtra)

x <- read.csv("02/ga.csv", as.is = TRUE)
x$num <- 1:length(x$T)
qplot(x$num, x$savg,
      geom=c("point", "smooth"),
      xlab='run', ylab='score', main='Score Distribution over Time'
      )

lm.0 <- lm(x$savg ~ x$T + x$A + x$B + x$Oc + x$C + x$Oj + x$tj + x$Dp + x$Dn)
summary(lm.0)


q1 <- qplot(x$A, x$savg, ylab='score', xlab='A')
q2 <- qplot(x$B, x$savg, ylab='score', xlab='B')
q3 <- qplot(x$T, x$savg, ylab='score', xlab='T')
q4 <- qplot(x$tj, x$savg, ylab='score', xlab='tj')

grid.arrange(q1,q2,q3,q4,ncol=2, top='Score vs different parameters')

q1 <- qplot(x$C, x$savg, ylab='score', xlab='C')
q2 <- qplot(x$Oc, x$savg, ylab='score', xlab='Oc')
q3 <- qplot(x$Dp, x$savg, ylab='score', xlab='Dp')
q4 <- qplot(x$Dn, x$savg, ylab='score', xlab='Dn')

grid.arrange(q1,q2,q3,q4,ncol=2, top='Score vs different parameters')

