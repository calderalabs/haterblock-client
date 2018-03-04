variable "domain" {
  default = "haterblock.io"
}

variable "bucket" {
  default = "haterblock-client"
}

variable "regions" {
  type = "map"

  default = {
    "eu" = "eu-west-1"
    "us" = "us-east-1"
  }
}
