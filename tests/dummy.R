library(RUnit)
library(rvle)
f <- rvle.open('dummy.vpz')

cnd <- rvle.condition_list(f)

checkEquals(cnd[1], "test")

# check the type of the port values vector
checkEquals(storage.mode(rvle.condition_show(f,"test", "string")),
            "character")
checkEquals(storage.mode(rvle.condition_show(f,"test", "double")),
            "double")
checkEquals(storage.mode(rvle.condition_show(f,"test", "int")),
            "integer")
checkEquals(storage.mode(rvle.condition_show(f,"test", "bool")),
            "logical")
checkEquals(storage.mode(rvle.condition_show(f,"test", "multi_string")),
            "character")
checkEquals(storage.mode(rvle.condition_show(f,"test", "multi_double")),
            "double")
checkEquals(storage.mode(rvle.condition_show(f,"test", "multi_int")),
            "integer")
checkEquals(storage.mode(rvle.condition_show(f,"test", "multi_bool")),
            "logical")

# check the case of unmanaged type
checkException(rvle.condition_show(f,"test", ",notmanaged"))
