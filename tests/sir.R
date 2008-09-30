library(RUnit)
library(rvle)
f <- rvle.open('sir.vpz')

cnd <- rvle.condition_list(f)
checkEquals(cnd[1], "cond_sir")

result <- rvle.run(f)

# check result of the view
checkEqualsNumeric(dim(result[[1]])[1], 2000)
checkEqualsNumeric(dim(result[[1]])[2], 4)

# get the first view
view1 <- result[[1]]
checkEqualsNumeric(view1[[1]][[2000]], 19.99, tolerance=1e-5)
checkEqualsNumeric(view1[[2]][[2000]], 0.1542165, tolerance=1e-5)
checkEqualsNumeric(view1[[3]][[2000]], 4.973883e+02, tolerance=1e-5)
checkEqualsNumeric(view1[[4]][[2000]], 3.457524, tolerance=1e-5)

# show port list
ports <- rvle.condition_port_list(f, cnd)
checkEquals(ports[1], "a")
checkEquals(ports[2], "active")
checkEquals(ports[3], "dependance")
checkEquals(ports[4], "r")
checkEquals(ports[5], "threshold")
checkEquals(ports[6], "variables")

# change condition
checkEqualsNumeric(rvle.condition_show(f, cnd, "a"), 0.5, tolerance=1e-5)
rvle.condition_set_real(f, cnd, "a", 0.6)

# get the first view
result <- rvle.run(f)
checkEqualsNumeric(dim(result[[1]])[1], 2000)
checkEqualsNumeric(dim(result[[1]])[2], 4)

view1 <- result[[1]]
checkEqualsNumeric(view1[[1]][[2000]], 19.99, tolerance=1e-5)
checkEqualsNumeric(view1[[2]][[2000]], 0.04369215, tolerance=1e-5)
checkEqualsNumeric(view1[[3]][[2000]], 4.927206e+02, tolerance=1e-5)
checkEqualsNumeric(view1[[4]][[2000]], 8.235717, tolerance=1e-5)

# check the seed
seed = rvle.experiment_get_seed(f)
checkEquals(seed, 12379843)
rvle.experiment_set_seed(f, 12345678)
seed = rvle.experiment_get_seed(f)
checkEquals(seed, 12345678)

# check the duration
duration = rvle.experiment_get_duration(f)
checkEqualsNumeric(duration, 20, tolerance=1e-5)
rvle.experiment_set_duration(f, 123.321)
duration = rvle.experiment_get_duration(f)
checkEqualsNumeric(duration, 123.321, tolerance=1e-5)

