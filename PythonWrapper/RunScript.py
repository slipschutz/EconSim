



import matplotlib.pyplot as plt
import libWrapper


def go():
    libWrapper.Run()

    x=libWrapper.GetData()
    for i in range(len(x)):
        x[i]=x[i]*1000

    price = plt.plot(x,"r--",label="Price")
    demand = plt.plot(libWrapper.GetDemands(),label="demand")
    supply=plt.plot(libWrapper.GetSupplies(),label="supply")

    plt.yscale('log')

    plt.legend()
    plt.show()


if __name__ == "__main__":
    go()

