function hadamardize(dims::Array{Int,2}, bitreverse::Bool)
    ntot = 0
    for i = 1:size(dims,2)
        n = dims[1,i]
        if !ispow2(n)
            throw(ArgumentError("non power-of-two Hadamard-transform length"))
        end
        ntot += trailing_zeros(n)
    end
    hdims = Array{Int}(undef, 3,ntot)
    j = 0
    for i = 1:size(dims,2)
        n = dims[1,i]
        is = dims[2,i]
        os = dims[3,i]
        log2n = trailing_zeros(n)
        display(log2n)
        krange = j+1:j+log2n
        display(krange)
        for k in krange
            hdims[1,k] = 2
            hdims[2,k] = is
            hdims[3,k] = os
            is *= 2
            os *= 2
        end
        if bitreverse
            hdims[3,krange] = reverse(hdims[3,krange])
        end
        j += log2n
    end
    return hdims
end

dims=Array{Int64}(undef, 3, 1);
# dims[1,1]=32
# dims[2,1]=1
# dims[3,1]=1
# display(hadamardize(dims, false))

dims[1,1]=16
dims[2,1]=1
dims[3,1]=1
display(hadamardize(dims, false))
