



import matplotlib.pyplot as plt
import libWrapper
import IPython

def go():
    libWrapper.UnitTests()

    libWrapper.Run()
    

    x=libWrapper.GetPriceData()
#    for i in range(len(x)):
#        x[i]=x[i]*1000

    volume = plt.plot(libWrapper.GetVolumeData(),"b*",label="volume")
    price = plt.plot(x,"ro",label="Price")
    people = plt.plot(libWrapper.GetPopulation(),label="Population")
    demand = plt.plot(libWrapper.GetDemands(),"g^",label="demand")
    supply=plt.plot(libWrapper.GetSupplies(),label="supply")

    plt.yscale('log')

    plt.legend()
    plt.show(block=False)

    IPython.embed()


if __name__ == "__main__":
    go()

