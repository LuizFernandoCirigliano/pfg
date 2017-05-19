library(ggplot2)
library(gridExtra)

num_to_gen <- function(num) {
  if (num <= 150) {
    return (1)
  }
  else {
    return (((num - 150)%/%120) + 2)
  }
}

folder <- "06/"

x <- read.csv(paste(folder,"ga.csv", sep=""), as.is = TRUE)
x$num <- 1:length(x$T)
x$gen <- 1
x$gen[151:length(x$num)] <- ((x$num[151:length(x$num)] - 150)%/%120 + 2)

agg <- aggregate(savg ~ gen, data=x, max)
best_gen <- merge(agg, x)
best_gen <- best_gen[!duplicated(best_gen$gen),]

routes_x <- c()
routes_y <- c()
routes_gen <- c()

for (num in best_gen$num) {
  file_name <- paste(folder,"rotas/teste",num,"_0",".csv", sep="")
  print(file_name)
  route <- read.csv(file_name, header=FALSE, col.names = c("x", "y", "z"))
  routes_x <- append(routes_x, route$x)
  routes_y <- append(routes_y, route$y)
  gen <- num_to_gen(num)
  routes_gen <- append(routes_gen, rep(gen, length(route$x)))
}

routes <- data.frame(x=routes_x, y=routes_y, gen=routes_gen, gen.f=as.factor(routes_gen))

png(paste(folder, "trajectory.png", sep=""), width=720, height=480, unit="px")
traj_plt <- ggplot(routes, aes(x=x, y=y-(gen-1)/2, color=gen.f)) + geom_jitter()
traj_plt + labs(title="Trajectory for best robot of each generation", y="y (m)", x="x (m)", colour = "Generation")
dev.off()

png(paste(folder, "boxplot.png", sep=""), width=720, height=480, unit="px")
traj_plt <- ggplot(x, aes(x=as.factor(gen), y=savg)) + geom_boxplot()
traj_plt + labs(title = "Score spread over generations", y="Score", x="Generation")
dev.off()

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

best <- x[x$savg==max(x$savg),]


routes_x <- c()
routes_y <- c()
routes_num <- c()
routes_t <- c()
# Teste Longo
for (num in 1:10) {
  filename <- paste("long/rotas/teste_",num,".csv", sep="")
  route <- read.csv(filename, header=FALSE, col.names = c("x", "y", "z"))
  routes_x <- append(routes_x, route$x)
  routes_y <- append(routes_y, route$y)
  routes_num <- append(routes_num, rep(num, length(route$x)))
  routes_t <- append(routes_t, round(length(route$x)*0.025, 0))
}
 
routes <- data.frame(x=routes_x, y=routes_y, num=routes_num, num.f=as.factor(routes_num))
traj_plt <- ggplot(routes, aes(x=x, y=y+(num*19), color=num.f)) + ylim(0, 200)
traj_plt <- traj_plt + geom_jitter()
traj_plt <- traj_plt + labs(title="Maximum Distance Test (10 runs)", y="y (m)", x="x (m)")
traj_plt + scale_colour_discrete(name="Duration of walk(s)", 
                                 labels=routes_t)



