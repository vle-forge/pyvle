library(RUnit)
library(rvle)
f <- rvle.open('dummy.vpz')

cnd <- rvle.listConditions(f)

checkEquals(cnd[1], "test")

# check the type of the port values vector
checkEquals(storage.mode(rvle.getConditionPortValues(f,"test", "string")),
            "character")
checkEquals(storage.mode(rvle.getConditionPortValues(f,"test", "double")),
            "double")
checkEquals(storage.mode(rvle.getConditionPortValues(f,"test", "int")),
            "integer")
checkEquals(storage.mode(rvle.getConditionPortValues(f,"test", "bool")),
            "logical")
checkEquals(storage.mode(rvle.getConditionPortValues(f,"test", "multi_string")),
            "character")
checkEquals(storage.mode(rvle.getConditionPortValues(f,"test", "multi_double")),
            "double")
checkEquals(storage.mode(rvle.getConditionPortValues(f,"test", "multi_int")),
            "integer")
checkEquals(storage.mode(rvle.getConditionPortValues(f,"test", "multi_bool")),
            "logical")

# check the case of unmanaged type
checkException(rvle.getConditionPortValues(f,"test", ",notmanaged"))
